#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
  UNICODE_LIMIT = 0x110000,
  LINE_CAPACITY = 4096,
};

enum unicode_flag : uint16_t {
  UNICODE_ALPHA = 1u << 0,
  UNICODE_CNTRL = 1u << 1,
  UNICODE_DIGIT = 1u << 2,
  UNICODE_LOWER = 1u << 3,
  UNICODE_PRINT = 1u << 4,
  UNICODE_SPACE = 1u << 5,
  UNICODE_UPPER = 1u << 6,
  UNICODE_XDIGIT = 1u << 7,
};

typedef struct {
  uint16_t flags;
  int32_t lower_delta;
  int32_t upper_delta;
} unicode_entry_t;

/* Prints an operation failure and returns false. */
static bool fail(const char *operation, const char *path) {
  fprintf(stderr, "unicode_gen: %s %s: %s\n", operation, path, strerror(errno));
  return false;
}

/* Parses an optional hexadecimal code point field. */
static uint32_t parse_optional_hex(const char *field) {
  if (*field == '\0')
    return 0;
  return (uint32_t)strtoul(field, NULL, 16);
}

/* Marks the category-derived properties for one inclusive range. */
static void mark_category(unicode_entry_t *entries, uint32_t first,
                          uint32_t last, const char *category) {
  for (uint32_t rune = first; rune <= last; ++rune) {
    if (strcmp(category, "Cc") == 0)
      entries[rune].flags |= UNICODE_CNTRL;
    if (strcmp(category, "Nd") == 0)
      entries[rune].flags |= UNICODE_DIGIT;
    if ((category[0] != 'C' && category[0] != 'Z') || rune == 0x20)
      entries[rune].flags |= UNICODE_PRINT;
  }
}

/* Parses UnicodeData.txt categories and simple case mappings. */
static bool read_unicode_data(const char *path, unicode_entry_t *entries) {
  FILE *input = fopen(path, "r");
  if (input == NULL)
    return fail("open", path);

  char line[LINE_CAPACITY];
  uint32_t range_first = 0;
  char range_category[3] = {0};

  while (fgets(line, sizeof(line), input) != NULL) {
    char *fields[15] = {0};
    size_t count = 0;
    char *cursor = line;

    while (count < 15) {
      fields[count++] = cursor;
      char *separator = strchr(cursor, ';');
      if (separator == NULL)
        break;
      *separator = '\0';
      cursor = separator + 1;
    }

    if (count != 15) {
      fclose(input);
      errno = EINVAL;
      return fail("parse", path);
    }

    fields[14][strcspn(fields[14], "\r\n")] = '\0';
    uint32_t rune = (uint32_t)strtoul(fields[0], NULL, 16);

    if (strstr(fields[1], ", First>") != NULL) {
      range_first = rune;
      memcpy(range_category, fields[2], sizeof(range_category));
      continue;
    }

    if (strstr(fields[1], ", Last>") != NULL) {
      mark_category(entries, range_first, rune, range_category);
      range_first = 0;
      continue;
    }

    mark_category(entries, rune, rune, fields[2]);
    uint32_t upper = parse_optional_hex(fields[12]);
    uint32_t lower = parse_optional_hex(fields[13]);
    if (upper != 0)
      entries[rune].upper_delta = (int32_t)((int64_t)upper - rune);
    if (lower != 0)
      entries[rune].lower_delta = (int32_t)((int64_t)lower - rune);
  }

  if (ferror(input) != 0) {
    fclose(input);
    return fail("read", path);
  }

  if (fclose(input) != 0)
    return fail("close", path);
  return true;
}

/* Marks one named property from a UCD property file. */
static void mark_property(unicode_entry_t *entries, uint32_t first,
                          uint32_t last, uint16_t flag) {
  for (uint32_t rune = first; rune <= last; ++rune)
    entries[rune].flags |= flag;
}

/* Resolves the properties used by Sigma and ignores all others. */
static uint16_t property_flag(const char *property) {
  if (strcmp(property, "Alphabetic") == 0)
    return UNICODE_ALPHA;
  if (strcmp(property, "Lowercase") == 0)
    return UNICODE_LOWER;
  if (strcmp(property, "Uppercase") == 0)
    return UNICODE_UPPER;
  if (strcmp(property, "White_Space") == 0)
    return UNICODE_SPACE;
  if (strcmp(property, "Hex_Digit") == 0)
    return UNICODE_XDIGIT;
  return 0;
}

/* Parses selected ranges from a UCD property file. */
static bool read_properties(const char *path, unicode_entry_t *entries) {
  FILE *input = fopen(path, "r");
  if (input == NULL)
    return fail("open", path);

  char line[LINE_CAPACITY];
  while (fgets(line, sizeof(line), input) != NULL) {
    char *comment = strchr(line, '#');
    if (comment != NULL)
      *comment = '\0';

    uint32_t first = 0;
    uint32_t last = 0;
    char property[64] = {0};
    int matched = sscanf(line, "%x..%x ; %63s", &first, &last, property);
    if (matched != 3) {
      matched = sscanf(line, "%x ; %63s", &first, property);
      last = first;
    }
    if (matched < 2)
      continue;

    uint16_t flag = property_flag(property);
    if (flag != 0)
      mark_property(entries, first, last, flag);
  }

  if (ferror(input) != 0) {
    fclose(input);
    return fail("read", path);
  }

  if (fclose(input) != 0)
    return fail("close", path);
  return true;
}

/* Writes one unsigned integer in little-endian byte order. */
static bool write_u32(FILE *output, uint32_t value) {
  uint8_t bytes[4] = {
      (uint8_t)value,
      (uint8_t)(value >> 8),
      (uint8_t)(value >> 16),
      (uint8_t)(value >> 24),
  };
  return fwrite(bytes, sizeof(bytes), 1, output) == 1;
}

/* Writes one unsigned integer in little-endian byte order. */
static bool write_u16(FILE *output, uint16_t value) {
  uint8_t bytes[2] = {(uint8_t)value, (uint8_t)(value >> 8)};
  return fwrite(bytes, sizeof(bytes), 1, output) == 1;
}

/* Returns true when two code points can share one generated record. */
static bool entries_equal(unicode_entry_t left, unicode_entry_t right) {
  return left.flags == right.flags && left.lower_delta == right.lower_delta &&
         left.upper_delta == right.upper_delta;
}

/* Writes one nonempty property and mapping range. */
static bool write_record(FILE *output, uint32_t first, uint32_t last,
                         unicode_entry_t entry) {
  return write_u32(output, first) && write_u32(output, last) &&
         write_u16(output, entry.flags) &&
         write_u32(output, (uint32_t)entry.lower_delta) &&
         write_u32(output, (uint32_t)entry.upper_delta);
}

/* Compresses the complete scalar table into ordered fixed-size records. */
static bool write_database(const char *path, const unicode_entry_t *entries) {
  FILE *output = fopen(path, "wb");
  if (output == NULL)
    return fail("open", path);

  for (uint32_t first = 0; first < UNICODE_LIMIT;) {
    unicode_entry_t entry = entries[first];
    uint32_t last = first;
    while (last + 1 < UNICODE_LIMIT && entries_equal(entry, entries[last + 1]))
      ++last;

    if ((entry.flags != 0 || entry.lower_delta != 0 ||
         entry.upper_delta != 0) &&
        !write_record(output, first, last, entry)) {
      fclose(output);
      return fail("write", path);
    }
    first = last + 1;
  }

  if (fclose(output) != 0)
    return fail("close", path);
  return true;
}

/* Generates unicode.bin from UnicodeData and the two property databases. */
int main(int argc, char **argv) {
  if (argc != 5) {
    fprintf(stderr,
            "usage: unicode_gen UnicodeData.txt DerivedCoreProperties.txt "
            "PropList.txt unicode.bin\n");
    return EXIT_FAILURE;
  }

  unicode_entry_t *entries = calloc(UNICODE_LIMIT, sizeof(*entries));
  if (entries == NULL) {
    fail("allocate", "Unicode table");
    return EXIT_FAILURE;
  }

  bool ok = read_unicode_data(argv[1], entries) &&
            read_properties(argv[2], entries) &&
            read_properties(argv[3], entries) &&
            write_database(argv[4], entries);
  free(entries);
  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
