#!/usr/bin/env ruby
# frozen_string_literal: true

# format_keymap.rb
# Reformats LAYOUT() macros so token positions approximate the physical keyboard.
# Tokens are placed at columns proportional to their key's x coordinate, and
# padded to a width proportional to the key's physical width.

require "json"
require "optparse"

# ---------------------------------------------------------------------------
# Label normalization (for base-layer comments; shared logic with render_keymap_ascii.rb)
# ---------------------------------------------------------------------------

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
  "KC__VOLUP"   => "VOL+",
  "KC__VOLDOWN" => "VOL-",
  "KC_MEDIA_PREV_TRACK" => "<==",
  "KC_MEDIA_NEXT_TRACK" => "==>",
}.freeze

PREFIX_STRIP = {
  "KC_"  => "",
  "RGB_" => "💡",
}.freeze

PATTERN_RULES = [
  [/^MO\((\d+)\)$/,                  ->(m) { "MO#{m[1]}" }],
  [/^OSL\((\d+)\)$/,                 ->(m) { "OSL#{m[1]}" }],
  [/^TO\((\d+)\)$/,                  ->(m) { "TO#{m[1]}" }],
  [/^DF\((\d+)\)$/,                  ->(m) { "DF#{m[1]}" }],
  [/^TG\((\d+)\)$/,                  ->(m) { "TG#{m[1]}" }],
  [/^TT\((\d+)\)$/,                  ->(m) { "TT#{m[1]}" }],
  [/^LT\((\d+),\s*([^)]+)\)$/,       ->(m) { "LT#{m[1]}/#{normalize_basic(m[2])}" }],
  [/^MT\(([^,]+),\s*([^)]+)\)$/,     ->(m) { "MT/#{normalize_basic(m[2])}" }],
].freeze

def normalize_basic(token)
  raw = token.to_s.strip
  return LABEL_MAP[raw] if LABEL_MAP.key?(raw)
  PREFIX_STRIP.each do |prefix, replacement|
    return "#{replacement}#{raw.delete_prefix(prefix)}" if raw.start_with?(prefix)
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

# Number of leading spaces to prepend to a label so its text aligns with the
# corresponding characters in the token. Only applies when a bare prefix was
# stripped (e.g. "KC_" → ""), so the remaining text lines up column-for-column.
def label_indent(token)
  raw = token.to_s.strip
  return 0 if LABEL_MAP.key?(raw)
  return 0 if PATTERN_RULES.any? { |regex, _| regex.match(raw) }
  PREFIX_STRIP.each do |prefix, replacement|
    return prefix.length if raw.start_with?(prefix) && replacement.empty?
  end
  0
end

# ---------------------------------------------------------------------------
# Shared parsing helpers (duplicated from render_keymap_ascii.rb)
# ---------------------------------------------------------------------------

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
      return source[start...index] if depth.zero?
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
    when "(" then depth += 1; current << char
    when ")" then depth -= 1 if depth.positive?; current << char
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

  tokens.map { |t| t.gsub(/\\\s*\n/, " ").gsub(/\A\s*\\\s*/, "").strip }.reject(&:empty?)
end

def extract_layer_tokens(keymap_source, layer_selector)
  pattern = /\[\s*([^\]]+)\s*\]\s*=\s*LAYOUT\s*\(/m
  keymap_source.to_enum(:scan, pattern).each do
    match = Regexp.last_match
    next unless match[1].strip == layer_selector

    body = parse_balanced_parentheses(keymap_source, match.end(0) - 1)
    raise "Could not parse LAYOUT(...) for layer #{layer_selector}" if body.nil?

    return split_top_level_csv(body)
  end
  raise "Layer #{layer_selector} not found"
end

def extract_all_layer_names(keymap_source)
  keymap_source.scan(/\[\s*([^\]]+)\s*\]\s*=\s*LAYOUT\s*\(/m).map { |m| m[0].strip }
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

# ---------------------------------------------------------------------------
# Formatting logic
# ---------------------------------------------------------------------------

CODE_INDENT    = "        "   # 8 spaces — standard keymap body indent
COMMENT_INDENT = "     // "   # 5 spaces + "// " = 8 chars, aligns labels with tokens

def compute_unit_width(keys, min_key_width = nil)
  auto = keys.map do |key|
    w = [key[:w], 0.25].max
    # +2: 1 for the comma, 1 for the minimum trailing space
    ((key[:token].length + 2).to_f / w).ceil
  end.max || 7

  result = [auto, 3].max
  min_key_width ? [result, min_key_width].max : result
end

def build_row_parts(row, unit_width, total_keys, &content_for)
  parts  = []
  cursor = 0

  row.each do |key|
    key_col = (key[:x] * unit_width).round
    key_end = ((key[:x] + key[:w]) * unit_width).round
    slot    = key_end - key_col

    gap = key_col - cursor
    parts << " " * gap if gap > 0

    parts << content_for.call(key, slot)
    cursor = key_col + slot
  end

  parts.join.rstrip
end

# Format one layer's tokens into lines that visually approximate the keyboard.
# label_tokens: array of tokens from the base layer (for comment headers); nil = no comments.
def format_layer_rows(tokens, geometry, unit_width, label_tokens: nil)
  total = tokens.length
  keys  = geometry.each_with_index.map { |geo, i| geo.merge(token: tokens[i], idx: i) }

  rows = keys.group_by { |k| k[:y] }.sort_by { |y, _| y }.map { |_, ks| ks.sort_by { |k| k[:x] } }

  lines = []
  rows.each do |row|
    if label_tokens
      lines << ""
      comment = build_row_parts(row, unit_width, total) do |key, slot|
        base_token = label_tokens[key[:idx]]
        indent     = label_indent(base_token)
        (" " * indent + normalize_label(base_token)).ljust(slot)
      end
      lines << COMMENT_INDENT + comment
    end

    code = build_row_parts(row, unit_width, total) do |key, slot|
      content = key[:idx] == total - 1 ? key[:token] : "#{key[:token]},"
      content.ljust(slot)
    end
    lines << CODE_INDENT + code
  end

  lines
end

def format_layer(layer_name, tokens, geometry, unit_width, label_tokens: nil)
  rows = format_layer_rows(tokens, geometry, unit_width, label_tokens: label_tokens)
  "    [#{layer_name}] = LAYOUT(\n#{rows.join("\n")}),"
end

# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

options = { layer: nil, min_key_width: nil, labels: true }

parser = OptionParser.new do |opts|
  opts.banner = "Usage: ruby util/format_keymap.rb --keymap <path> --info <path> [options]"
  opts.separator ""
  opts.separator "  If --layer is omitted, all layers are formatted."
  opts.separator ""
  opts.on("--keymap PATH",        "Path to keymap.c")                                          { |v| options[:keymap] = v }
  opts.on("--info PATH",          "Path to keyboard info.json")                                 { |v| options[:info] = v }
  opts.on("--layer VALUE",        "Layer to format (e.g. L_BASE). Omit for all layers.")        { |v| options[:layer] = v }
  opts.on("--min-key-width N", Integer, "Minimum inner width for a 1u key")                     { |v| options[:min_key_width] = v }
  opts.on("--no-labels",          "Omit base-layer key label comments above each row")          { options[:labels] = false }
end

begin
  parser.parse!
  raise "--keymap is required" unless options[:keymap]
  raise "--info is required"   unless options[:info]
rescue => e
  warn "Error: #{e.message}"
  warn parser.banner
  exit 1
end

begin
  keymap_source = File.read(options[:keymap])
  geometry      = load_geometry(options[:info])
  all_names     = extract_all_layer_names(keymap_source)

  # Load base layer tokens for label comments (first layer in file)
  label_tokens = if options[:labels] && all_names.any?
    extract_layer_tokens(keymap_source, all_names.first)
  end

  requested   = options[:layer]
  layer_names = (requested.nil? || requested.downcase == "all") ? all_names : [requested]
  raise "No layers found" if layer_names.empty?

  output_blocks = layer_names.map do |name|
    tokens     = extract_layer_tokens(keymap_source, name)
    keys       = geometry.each_with_index.map { |geo, i| geo.merge(token: tokens[i]) }
    unit_width = compute_unit_width(keys, options[:min_key_width])
    $stderr.puts "#{name}: unit_width=#{unit_width}"
    format_layer(name, tokens, geometry, unit_width, label_tokens: label_tokens)
  end

  puts output_blocks.join("\n\n")
rescue => e
  warn "Error: #{e.message}"
  exit 1
end
