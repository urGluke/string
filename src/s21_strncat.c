#include "s21_string.h"

char *s21_strncat(
    char *dest, const char *src,
    s21_size_t n)  // Добавляет строку, на которую указывает src, в конец
                   // строки, на которую указывает dest, длиной до n символов.
{
  s21_size_t size = s21_strlen(dest);
  s21_size_t i;
  for (i = 0; *(src + i) != '\0' && i < n; i++) {
    *(dest + size + i) = *(src + i);
  }
  *(dest + size + i) = '\0';
  return dest;
}
