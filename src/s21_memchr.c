#include "s21_string.h"

void *s21_memchr(const void *str, int c,
                 s21_size_t n)  //Выполняет поиск первого вхождения символа c
                                //(беззнаковый тип)
//в первых n байтах строки, на которую указывает аргумент str.
{
  char *find = s21_NULL;
  for (s21_size_t i = 0; i < n; i++) {
    if (*((char *)str + i) == c) {
      find = (char *)str + i;
      break;
    }
  }
  return find;
}