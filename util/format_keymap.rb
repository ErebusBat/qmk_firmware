#!/usr/bin/env ruby
# frozen_string_literal: true

# format_keymap.rb
# Reformats LAYOUT() macros so token positions approximate the physical keyboard.
# Tokens are placed at columns proportional to their key's x coordinate, and
# padded to a width proportional to the key's physical width.

require "json"
require "optparse"

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

def compute_unit_width(keys, min_key_width = nil)
  auto = keys.map do |key|
    w = [key[:w], 0.25].max
    # +2: 1 for the comma, 1 for the minimum trailing space
    ((key[:token].length + 2).to_f / w).ceil
  end.max || 7

  result = [auto, 3].max
  min_key_width ? [result, min_key_width].max : result
end

# Format one layer's tokens into lines that visually approximate the keyboard.
# Returns an array of strings (one per row), without a surrounding wrapper.
def format_layer_rows(tokens, geometry, unit_width)
  total = tokens.length
  keys = geometry.each_with_index.map { |geo, i| geo.merge(token: tokens[i], idx: i) }

  rows = keys.group_by { |k| k[:y] }.sort_by { |y, _| y }.map { |_, ks| ks.sort_by { |k| k[:x] } }

  lines = []
  rows.each do |row|
    parts = []
    cursor = 0  # current column (in unit_width units * actual chars)

    row.each do |key|
      is_last  = key[:idx] == total - 1
      key_col  = (key[:x] * unit_width).round
      key_end  = ((key[:x] + key[:w]) * unit_width).round
      slot     = key_end - key_col
      content  = is_last ? key[:token] : "#{key[:token]},"

      # Fill any gap between cursor and this key's expected column
      gap = key_col - cursor
      parts << " " * gap if gap > 0

      # Pad content to slot width (unit_width guarantees content.length < slot)
      parts << content.ljust(slot)
      cursor = key_col + slot
    end

    lines << parts.join.rstrip
  end

  lines
end

def format_layer(layer_name, tokens, geometry, unit_width, indent: "        ")
  rows = format_layer_rows(tokens, geometry, unit_width)
  body = rows.map { |r| indent + r }.join("\n")
  "    [#{layer_name}] = LAYOUT(\n#{body}),"
end

# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

options = { layer: nil, min_key_width: nil }

parser = OptionParser.new do |opts|
  opts.banner = "Usage: ruby util/format_keymap.rb --keymap <path> --info <path> [--layer <selector>] [--min-key-width <n>]"
  opts.separator ""
  opts.separator "  If --layer is omitted, all layers are formatted."
  opts.separator ""
  opts.on("--keymap PATH", "Path to keymap.c") { |v| options[:keymap] = v }
  opts.on("--info PATH",   "Path to keyboard info.json") { |v| options[:info] = v }
  opts.on("--layer VALUE", "Layer to format (e.g. L_BASE). Omit for all layers.") { |v| options[:layer] = v }
  opts.on("--min-key-width N", Integer, "Minimum inner width for a 1u key") { |v| options[:min_key_width] = v }
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

  layer_names = options[:layer] ? [options[:layer]] : extract_all_layer_names(keymap_source)
  raise "No layers found" if layer_names.empty?

  output_blocks = layer_names.map do |name|
    tokens     = extract_layer_tokens(keymap_source, name)
    keys       = geometry.each_with_index.map { |geo, i| geo.merge(token: tokens[i]) }
    unit_width = compute_unit_width(keys, options[:min_key_width])
    $stderr.puts "#{name}: unit_width=#{unit_width}"
    format_layer(name, tokens, geometry, unit_width)
  end

  puts output_blocks.join("\n\n")
rescue => e
  warn "Error: #{e.message}"
  exit 1
end
