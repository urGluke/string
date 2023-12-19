#include "s21_string.h"

char *s21_strchr(
    const char *str,
    int c)  // Выполняет поиск первого вхождения символа c (беззнаковый
            // тип) в строке, на которую указывает аргумент str.
{
  char *find;
  char *s = (char *)str;
  for (; *s != '\0' && *s != c; s++) {
  }
  if (*s == '\0' && c != '\0') {
    find = s21_NULL;
  } else {
    find = s;
  }
  return find;
}
