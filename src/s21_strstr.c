#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  const char *result = s21_NULL;
  const char *string_main = s21_NULL;
  const char *string_destination = s21_NULL;
  s21_size_t string_len = 0, count = 0, count2 = 0;
  int found = 0;

  s21_size_t needle_len = s21_strlen(needle);

  if (needle_len == 0) {
    return (char *)haystack;
  }

  if (haystack == s21_NULL || needle == s21_NULL) return s21_NULL;  // exit(1);

  string_main = haystack;
  string_destination = needle;
  string_len = s21_strlen(string_destination);

  while (*string_main != '\0' && !found) {
    if (*string_main == *string_destination) {
      if (count == (string_len - 1))
        found = 1;
      else
        found = 0;

      count += 1;
      string_destination += 1;
      string_main += 1;
    } else {
      count2 += 1;
      string_main = haystack;
      string_main += count2;
      count = 0;
      string_destination = needle;
    }
  }

  if (found == 1) result = string_main - count;

  return (char *)result;
}
