#!/usr/bin/env ruby
# frozen_string_literal: true

require "json"
require "optparse"

LABEL_MAP = {
  "KC_TRNS" => "    ",
  "_______" => "    ",
  "XXXXXXX" => "",
  "KC_GRV"  => "~",
  "KC_COMM" => ",<",
  "KC_DOT"  => ".>",
  "KC_SLSH" => "/?",
  "KC_SCLN" => ";:",
  "KC_QUOT" => "'\"",
  "KC_MINS" => "-",
  "KC_EQL"  => "=+",
  "KC_MPLY" => "PLAY",
  "KC_MSTP" => "STOP",
  "KC_MPRV" => "PREV",
  "KC_MNXT" => "NEXT",
  "KC_MUTE" => "MUTE",
  "KC_VOLU" => "VOL+",
  "KC_VOLD" => "VOL-",
  "KC__VOLUP" => "VOL+",
  "KC__VOLDOWN" => "VOL-",
  "KC_MEDIA_PREV_TRACK" => "<==",
  "KC_MEDIA_NEXT_TRACK" => "==>",
}.freeze

PREFIX_STRIP = {
  "KC_" => "",
  "RGB_" => "💡"
}.freeze

PATTERN_RULES = [
  [/^MO\((\d+)\)$/, ->(m) { "MO#{m[1]}" }],
  [/^OSL\((\d+)\)$/, ->(m) { "OSL#{m[1]}" }],
  [/^TO\((\d+)\)$/, ->(m) { "TO#{m[1]}" }],
  [/^DF\((\d+)\)$/, ->(m) { "DF#{m[1]}" }],
  [/^TG\((\d+)\)$/, ->(m) { "TG#{m[1]}" }],
  [/^TT\((\d+)\)$/, ->(m) { "TT#{m[1]}" }],
  [/^LT\((\d+),\s*([^)]+)\)$/, ->(m) { "LT#{m[1]}/#{normalize_basic(m[2])}" }],
  [/^MT\(([^,]+),\s*([^)]+)\)$/, ->(m) { "MT/#{normalize_basic(m[2])}" }]
].freeze

# Compute the display width of a string, accounting for wide characters (e.g. emoji).
# East Asian wide/fullwidth characters and most emoji occupy 2 terminal columns.
def char_width(cp)
  if cp >= 0x1F000 ||            # emoji and symbols
     cp >= 0x2300 && cp <= 0x23FF || # misc technical (⏮, ⏭, ⏯, etc.)
     cp >= 0x2600 && cp <= 0x27BF || # misc symbols, dingbats
     cp >= 0x2B50 && cp <= 0x2B55 || # stars etc
     cp >= 0x1F300 && cp <= 0x1FAFF   # emoticons, transport, etc
    2
  else
    1
  end
end

def display_width(str)
  chars = str.chars
  total = 0
  i = 0
  while i < chars.length
    cp = chars[i].ord
    next_cp = chars[i + 1]&.ord
    if next_cp == 0xFE0E # VS15: text presentation → 1 column
      total += 1
      i += 2
    elsif next_cp == 0xFE0F # VS16: emoji presentation → 2 columns
      total += 2
      i += 2
    elsif cp >= 0xFE00 && cp <= 0xFE0F # standalone variation selector
      i += 1
    else
      total += char_width(cp)
      i += 1
    end
  end
  total
end

# Center a string within `width` display columns, padding with spaces.
def display_center(str, width)
  dw = display_width(str)
  return str if dw >= width
  total_pad = width - dw
  left_pad = total_pad / 2
  right_pad = total_pad - left_pad
  "#{' ' * left_pad}#{str}#{' ' * right_pad}"
end

def normalize_basic(token)
  raw = token.to_s.strip
  return LABEL_MAP[raw] if LABEL_MAP.key?(raw)

  PREFIX_STRIP.each do |prefix, replacement|
    if raw.start_with?(prefix)
      return "#{replacement}#{raw.delete_prefix(prefix)}"
    end
  end

  raw
end

def normalize_label(token)
  raw = token.to_s.strip
  return LABEL_MAP[raw] if LABEL_MAP.key?(raw)

  PATTERN_RULES.each do |regex, formatter|
    match = regex.match(raw)
    return formatter.call(match) if match
  end

  normalize_basic(raw)
end

def resolve_layer_selector(value)
  str = value.to_s.strip
  raise "Layer selector must not be empty" if str.empty?

  str
end

def strip_c_comments(source)
  source
    .gsub(%r{/\*.*?\*/}m, "")
    .gsub(%r{//.*$}, "")
end

def parse_balanced_parentheses(source, open_paren_index)
  depth = 0
  index = open_paren_index
  start = open_paren_index + 1

  while index < source.length
    char = source[index]
    if char == "("
      depth += 1
    elsif char == ")"
      depth -= 1
      if depth.zero?
        return source[start...index]
      end
    end
    index += 1
  end

  nil
end

def split_top_level_csv(body)
  body = strip_c_comments(body)
  tokens = []
  current = +""
  depth = 0

  body.each_char do |char|
    case char
    when "("
      depth += 1
      current << char
    when ")"
      depth -= 1 if depth.positive?
      current << char
    when ","
      if depth.zero?
        tokens << current
        current = +""
      else
        current << char
      end
    else
      current << char
    end
  end

  tokens << current unless current.empty?

  tokens.map do |token|
    token
      .gsub(/\\\s*\n/, " ")
      .gsub(/\A\s*\\\s*/, "")
      .strip
  end.reject(&:empty?)
end

def extract_layer_tokens(keymap_source, layer_selector)
  pattern = /\[\s*([^\]]+)\s*\]\s*=\s*LAYOUT\s*\(/m
  keymap_source.to_enum(:scan, pattern).each do
    match = Regexp.last_match
    current_layer = match[1].strip
    next unless current_layer == layer_selector

    open_paren_index = match.end(0) - 1
    body = parse_balanced_parentheses(keymap_source, open_paren_index)
    raise "Could not parse LAYOUT(...) for layer #{layer_selector}" if body.nil?

    return split_top_level_csv(body)
  end

  raise "Layer #{layer_selector} not found"
end

def load_geometry(info_path)
  data = JSON.parse(File.read(info_path))
  layout = data.dig("layouts", "LAYOUT", "layout")
  raise "Could not find layouts.LAYOUT.layout in #{info_path}" unless layout.is_a?(Array)

  layout.map do |entry|
    {
      x: entry.fetch("x").to_f,
      y: entry.fetch("y").to_f,
      w: (entry["w"] || 1).to_f
    }
  end
end

def compute_unit_width(keys, min_key_width, token_width: false)
  auto = keys.map do |key|
    width = key[:w]
    width = 1.0 if width <= 0
    label = token_width ? key[:token] : key[:label]
    (display_width(label).to_f / width).ceil
  end.max || 3

  auto = [auto, 3].max
  return auto if min_key_width.nil?

  [auto, min_key_width].max
end

def render_rows(keys, unit_width, blank: false)
  unit_pitch = unit_width + 1
  grouped = keys.group_by { |key| key[:y] }
  sorted_rows = grouped.keys.sort
  output = []

  sorted_rows.each do |row_y|
    row_keys = grouped[row_y].sort_by { |key| key[:x] }

    prepared = row_keys.map do |key|
      total_width = [(key[:w] * unit_pitch).round + 1, 3].max
      inner = total_width - 2
      label = key[:label][0, inner]
      left = (key[:x] * unit_pitch).round + 1

      {
        left: left,
        inner: inner,
        right: left + total_width - 1,
        label: label
      }
    end

    row_width = prepared.map { |key| key[:right] }.max.to_i + 1
    top = Array.new(row_width, " ")
    mid = Array.new(row_width, " ")
    bot = Array.new(row_width, " ")

    prepared.each do |key|
      left = key[:left]
      right = key[:right]
      inner = key[:inner]

      top[left] = "+"
      bot[left] = "+"
      top[right] = "+"
      bot[right] = "+"

      (left + 1...right).each do |i|
        top[i] = "-"
        bot[i] = "-"
      end

      mid[left] = "|"
      mid[right] = "|"

      next if blank

      padded = display_center(key[:label], inner)
      chars = padded.chars
      col = left + 1
      ci = 0
      while ci < chars.length
        ch = chars[ci]
        cp = ch.ord
        next_cp = chars[ci + 1]&.ord

        if cp >= 0xFE00 && cp <= 0xFE0F
          # Stray variation selector: append to previous cell
          mid[col - 1] = "#{mid[col - 1]}#{ch}" if col > left + 1
          ci += 1
        elsif next_cp == 0xFE0E
          # Text presentation: base + VS15 = 1 column
          mid[col] = "#{ch}#{chars[ci + 1]}"
          col += 1
          ci += 2
        elsif next_cp == 0xFE0F
          # Emoji presentation: base + VS16 = 2 columns
          mid[col] = "#{ch}#{chars[ci + 1]}"
          col += 1
          mid[col] = "" if col <= right - 1
          col += 1
          ci += 2
        else
          cw = char_width(cp)
          mid[col] = ch
          col += 1
          if cw == 2
            mid[col] = "" if col <= right - 1
            col += 1
          end
          ci += 1
        end
      end
    end

    output << top.join.rstrip
    output << mid.join.rstrip
    output << bot.join.rstrip
  end

  output.join("\n")
end

options = {
  layer: "1",
  min_key_width: nil,
  token_width: false,
  blank: false
}

parser = OptionParser.new do |opts|
  opts.banner = "Usage: ruby util/render_keymap_ascii.rb --keymap <path> --info <path> [--layer <selector>] [--min-key-width <n>]"

  opts.on("--keymap PATH", "Path to keymap.c") { |v| options[:keymap] = v }
  opts.on("--info PATH", "Path to keyboard info.json") { |v| options[:info] = v }
  opts.on("--layer VALUE", "Layer selector exactly as in keymap, e.g. 1 or L_BASE") { |v| options[:layer] = v }
  opts.on("--min-key-width N", Integer, "Minimum inner width for a 1u key") { |v| options[:min_key_width] = v }
  opts.on("--token-width", "Size cells based on raw keymap token width instead of display labels") { options[:token_width] = true }
  opts.on("--blank", "Render box layout with empty key labels") { options[:blank] = true }
end

begin
  parser.parse!

  if options[:keymap].nil? || options[:info].nil?
    warn parser.banner
    exit 1
  end

  layer_selector = resolve_layer_selector(options[:layer])

  keymap_source = File.read(options[:keymap])
  tokens = extract_layer_tokens(keymap_source, layer_selector)
  geometry = load_geometry(options[:info])

  if tokens.length != geometry.length
    warn "Token count (#{tokens.length}) does not match geometry count (#{geometry.length})"
    exit 1
  end

  keys = geometry.each_with_index.map do |geo, idx|
    geo.merge(token: tokens[idx], label: normalize_label(tokens[idx]))
  end

  token_unit_width = compute_unit_width(keys, options[:min_key_width], token_width: true)
  unit_width = compute_unit_width(keys, options[:min_key_width], token_width: options[:token_width])
  $stderr.puts("effective_min_key_width=#{unit_width}")
  $stderr.puts("token_min_key_width=#{token_unit_width}")
  $stderr.puts("passed_min_key_width=#{options[:min_key_width]}") unless options[:min_key_width].nil?
  puts render_rows(keys, unit_width, blank: options[:blank])
rescue StandardError => e
  warn "Error: #{e.message}"
  exit 1
end
