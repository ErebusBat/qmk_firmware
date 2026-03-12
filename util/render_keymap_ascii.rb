#!/usr/bin/env ruby
# frozen_string_literal: true

require "json"
require "optparse"

LABEL_MAP = {
  "_______" => "TRNS",
  "XXXXXXX" => "NO",
  "KC_MPLY" => "PLAY",
  "KC_MSTP" => "STOP",
  "KC_MPRV" => "PREV",
  "KC_MNXT" => "NEXT",
  "KC_MUTE" => "MUTE",
  "KC_VOLU" => "VOL+",
  "KC_VOLD" => "VOL-",
  "KC__VOLUP" => "VOL+",
  "KC__VOLDOWN" => "VOL-"
}.freeze

PREFIX_STRIP = {
  "KC_" => "",
  "RGB_" => "RGB_"
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

def compute_unit_width(keys, min_key_width)
  auto = keys.map do |key|
    width = key[:w]
    width = 1.0 if width <= 0
    (key[:label].length.to_f / width).ceil
  end.max || 3

  auto = [auto, 3].max
  return auto if min_key_width.nil?

  [auto, min_key_width].max
end

def render_rows(keys, unit_width)
  unit_pitch = unit_width + 1
  grouped = keys.group_by { |key| key[:y] }
  sorted_rows = grouped.keys.sort
  output = []

  sorted_rows.each_with_index do |row_y, index|
    row_keys = grouped[row_y].sort_by { |key| key[:x] }

    prepared = []
    row_keys.each_with_index do |key, key_index|
      inner = [(key[:w] * unit_width).round, 1].max
      label = key[:label][0, inner]

      left = if key_index.zero?
               (key[:x] * unit_pitch).round + 1
             else
               prev_key = row_keys[key_index - 1]
               prev_prepared = prepared[key_index - 1]
               gap_units = key[:x] - (prev_key[:x] + prev_key[:w])
               gap_chars = (gap_units * unit_pitch).round
               prev_prepared[:right] + [gap_chars, 0].max
             end

      prepared << {
        left: left,
        inner: inner,
        right: left + inner + 1,
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

      padded = key[:label].center(inner)
      padded.chars.each_with_index do |char, offset|
        mid[left + 1 + offset] = char
      end
    end

    output << top.join.rstrip
    output << mid.join.rstrip
    output << bot.join.rstrip
    output << "" unless index == sorted_rows.length - 1
  end

  output.join("\n")
end

options = {
  layer: "1",
  min_key_width: nil
}

parser = OptionParser.new do |opts|
  opts.banner = "Usage: ruby util/render_keymap_ascii.rb --keymap <path> --info <path> [--layer <selector>] [--min-key-width <n>]"

  opts.on("--keymap PATH", "Path to keymap.c") { |v| options[:keymap] = v }
  opts.on("--info PATH", "Path to keyboard info.json") { |v| options[:info] = v }
  opts.on("--layer VALUE", "Layer selector exactly as in keymap, e.g. 1 or L_BASE") { |v| options[:layer] = v }
  opts.on("--min-key-width N", Integer, "Minimum inner width for a 1u key") { |v| options[:min_key_width] = v }
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

  unit_width = compute_unit_width(keys, options[:min_key_width])
  puts render_rows(keys, unit_width)
rescue StandardError => e
  warn "Error: #{e.message}"
  exit 1
end
