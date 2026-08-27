#!/bin/sh

set -eu

readme=${SIGMA_STATS_README:-README.md}
start='<!-- sigma-code-stats:start -->'
end='<!-- sigma-code-stats:end -->'
stats=$(find include src -type f \( -name '*.c' -o -name '*.h' \) \
  -exec awk '
    function without_comments(line, start, rest, close_at) {
      rest = line
      result = ""

      while (length(rest)) {
        if (block_comment) {
          close_at = index(rest, "*/")
          if (!close_at) return result
          rest = substr(rest, close_at + 2)
          block_comment = 0
          continue
        }

        start = index(rest, "/*")
        if (!start) {
          result = result rest
          break
        }

        result = result substr(rest, 1, start - 1)
        rest = substr(rest, start + 2)
        block_comment = 1
      }

      sub(/\/\/.*$/, "", result)
      return result
    }

    {
      line = $0
      code = without_comments(line)

      if (macro) {
        macro_lines++
        macro = line ~ /\\[[:space:]]*$/
        next
      }

      if (code ~ /^[[:space:]]*#[[:space:]]*define([[:space:]]|$)/) {
        macro_lines++
        macro = line ~ /\\[[:space:]]*$/
        next
      }

      if (code ~ /^[[:space:]]*#/) next
      if (code ~ /[^[:space:]]/) c_lines++
    }

    END {
      total = macro_lines + c_lines
      macro_percent = total ? 100 * macro_lines / total : 0
      c_percent = total ? 100 * c_lines / total : 0
      printf "%d %.1f %d %.1f %d\n", macro_lines, macro_percent, \
             c_lines, c_percent, total
    }
  ' {} +)

set -- $stats
macro_lines=$1
macro_percent=$2
c_lines=$3
c_percent=$4
total=$5

replacement=$(printf '%s\n\n%s\n%s\n%s\n%s\n%s\n\n%s' \
  "$start" \
  '| Kind | Lines | Share |' \
  '| --- | ---: | ---: |' \
  "| C23 macros | $macro_lines | $macro_percent% |" \
  "| Runtime C and declarations | $c_lines | $c_percent% |" \
  "| Total classified code | $total | 100.0% |" \
  "$end")

temp=$(mktemp "${readme}.stats.XXXXXX")
awk -v start="$start" -v end="$end" -v replacement="$replacement" '
  $0 == start {
    print replacement
    replacing = 1
    found = 1
    next
  }
  $0 == end {
    replacing = 0
    next
  }
  !replacing { print }
  END { if (!found) exit 2 }
' "$readme" >"$temp"
mv "$temp" "$readme"

printf 'Updated %s: macros %s%%, C %s%% (%s classified lines).\n' \
  "$readme" "$macro_percent" "$c_percent" "$total"
