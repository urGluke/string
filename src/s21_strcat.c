#include "s21_string.h"

char *s21_strcat(
    char *dest,
    const char *src)  // Добавляет строку, на которую указывает src,
                      // в конец строки, на которую указывает dest.
{
  s21_size_t size = s21_strlen(dest);
  s21_size_t i;
  for (i = 0; *(src + i) != '\0'; i++) {
    *(dest + size + i) = *(src + i);
  }
  *(dest + size + i) = '\0';
  return dest;
}
