#!/bin/sh

fixture=${SIGMA_DIAGNOSTICS_FIXTURE:-tests/compile_fail/diagnostics.c}
zig_bin=${ZIG:-zig}

output=$(
  "$zig_bin" cc -std=c23 -Iinclude -I../sigma_malloc/include -c "$fixture" \
    -o /dev/null 2>&1
)
result=$?

if [ "$result" -eq 0 ]; then
  printf '%s\n' 'diagnostic fixture unexpectedly compiled'
  exit 1
fi

for marker in \
  SIGMA_E_FORMAT_TYPE \
  SIGMA_E_MOVE_TYPE \
  SIGMA_E_CHARACTER_TYPE \
  SIGMA_E_DEINIT_TYPE \
  ' = help: '
do
  if ! printf '%s' "$output" | grep -Fq "$marker"; then
    printf '%s\n' "$output"
    printf '\nmissing diagnostic marker: %s\n' "$marker"
    exit 1
  fi
done

if [ "${SIGMA_DIAGNOSTICS_COLOR:-always}" = never ]; then
  bold=
  red=
  pink=
  blue=
  cyan=
  reset=
else
  bold='\033[1m'
  red='\033[1;31m'
  pink='\033[1;35m'
  blue='\033[1;34m'
  cyan='\033[1;36m'
  reset='\033[0m'
fi

show_diagnostic() {
  namespace=$1
  summary=$2
  pattern=$3
  note=$4
  help=$5
  location=$(grep -nF "$pattern" "$fixture" | head -n 1)
  line_number=${location%%:*}
  line=${location#*:}
  column=$(awk -v line="$line" -v pattern="$pattern" \
    'BEGIN { print index(line, pattern) }')
  carets=$(awk -v pattern="$pattern" \
    'BEGIN { for (i = 0; i < length(pattern); i++) printf "^" }')

  printf '%bError:%b %b%s%b\n\n' "$bold" "$reset" "$pink" "$namespace" "$reset"
  printf ' %b×%b %b%s%b\n' "$red" "$reset" "$bold" "$summary" "$reset"
  printf '   %b╭─[%b%b%s:%s:%s%b%b]%b\n' \
    "$blue" "$reset" "$bold" "$fixture" "$line_number" "$column" \
    "$reset" "$blue" "$reset"
  printf '%2s %b│%b %s\n' "$line_number" "$blue" "$reset" "$line"
  if [ "$column" -ge 6 ]; then
    hook_padding=$(awk -v column="$column" \
      'BEGIN { for (i = 6; i < column; i++) printf " " }')
    printf '   %b·%b %s%b╰─── %b%s%b\n' \
      "$blue" "$reset" "$hook_padding" "$red" "$bold" "$carets" "$reset"
  else
    caret_padding=$(awk -v column="$column" \
      'BEGIN { for (i = 1; i < column; i++) printf " " }')
    printf '   %b·%b %s%b%b%s%b\n' \
      "$blue" "$reset" "$caret_padding" "$red" "$bold" "$carets" "$reset"
  fi
  printf '\n   %bnote:%b %s\n' "$blue" "$reset" "$note"
  printf '\n   %bhelp:%b %s\n\n' "$cyan" "$reset" "$help"
}

# Return the declared type of a local variable before the diagnostic line.
declared_type() {
  name=$1
  before_line=$2
  awk -v name="$name" -v before_line="$before_line" '
    NR >= before_line { exit }
    $0 ~ "^[[:space:]]*[[:alnum:]_]+[[:space:]]+" name \
           "([[:space:]]*=|[[:space:]]*;)" {
      sub(/^[[:space:]]*/, "")
      print $1
      found = 1
    }
    END { if (!found) print "unknown type" }
  ' "$fixture" | tail -n 1
}

format_line=$(grep -nF 'sigma_fmt_arg((' "$fixture" | head -n 1)
format_source=${format_line#*:}
format_pattern=$(printf '%s\n' "$format_source" | sed -n \
  's/.*\(sigma_fmt_arg(([^;]*)\).*/\1/p')
format_type=$(printf '%s\n' "$format_source" | sed -n \
  's/.*sigma_fmt_arg((\([[:alnum:]_]*\)){.*/\1/p')

move_line=$(grep -nF 'SIGMA_MOVE(destination, source)' "$fixture" | head -n 1)
move_line_number=${move_line%%:*}
destination_type=$(declared_type destination "$move_line_number")
source_type=$(declared_type source "$move_line_number")

character_line=$(grep -nF 'sigma_isalpha(' "$fixture" | head -n 1)
character_source=${character_line#*:}
character_argument=$(printf '%s\n' "$character_source" | sed -n \
  's/.*sigma_isalpha(\([^)]*\)).*/\1/p')
case $character_argument in
  *.*f | *.*F) character_type=float ;;
  *.*) character_type=double ;;
  *) character_type='integer type' ;;
esac

deinit_line=$(grep -nF 'sigma_deinit(value)' "$fixture" | head -n 1)
deinit_line_number=${deinit_line%%:*}
deinit_type=$(declared_type value "$deinit_line_number")

show_diagnostic \
  'sigma::compile::format_type' \
  'Unsupported formatting argument type' \
  "$format_pattern" \
  "Found $format_type. Sigma only accepts builtin or explicitly registered formatting types." \
  "Add X(MY_TYPE, $format_type, my_formatter) to SIGMA_CUSTOM_FORMAT_TYPES before including sigma/printf.h."

show_diagnostic \
  'sigma::compile::move_type' \
  'Cannot move between different types' \
  'SIGMA_MOVE(destination, source)' \
  "destination is $destination_type while source is $source_type. SIGMA_MOVE transfers ownership and never performs conversion." \
  "Convert source into $destination_type first, or change destination to $source_type."

show_diagnostic \
  'sigma::compile::character_type' \
  'Unsupported character operation type' \
  'sigma_isalpha(1.0)' \
  "Found $character_type. Character traits dispatch only byte characters and sigma_rune values." \
  'Pass char, signed char, unsigned char, or sigma_rune.'

show_diagnostic \
  'sigma::compile::deinit_type' \
  'Unsupported owning type for sigma_deinit' \
  'sigma_deinit(value)' \
  "Found $deinit_type. Plain scalar values own no resource and must not be deinitialized." \
  'Remove this call, or register an owning type with its clone and deinit functions in SIGMA_OWNED_TYPES.'

if [ -n "${SIGMA_DIAGNOSTICS_RAW:-}" ]; then
  printf '%bCompiler output:%b\n%s\n' "$bold" "$reset" "$output"
fi

printf '%b✓ All compile-time diagnostics matched.%b\n' "$bold" "$reset"
