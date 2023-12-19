#include "s21_string.h"

char *s21_strrchr(
    const char *str,
    int c)  // Выполняет поиск последнего вхождения символа c (беззнаковый тип)
            // в строке, на которую указывает аргумент str.
{
  const char *find = s21_NULL;
  if (str != s21_NULL) {
    do {
      if (*str == (char)c) {
        find = str;
      }
    } while (*str++);
  }
  return (char *)find;
}
