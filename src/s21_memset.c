#include "s21_string.h"

void *s21_memset(
    void *str, int c,
    s21_size_t n)  // Копирует символ c (беззнаковый тип) в первые n
                   // символов строки, на которую указывает аргумент str.
{
  for (s21_size_t i = 0; i < n; i++) {
    *((char *)str + i) = c;
  }
  return str;
}
