#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  char const *c_string1 = (char const *)str1;
  char const *c_string2 = (char const *)str2;
  int find = 0;
  for (int i = 0; i < (int)n; i++) {
    if (*c_string1 != *c_string2) {
      find = *c_string1 - *c_string2;
      break;
    }
    c_string1 += 1;
    c_string2 += 1;
  }
  return find;
}