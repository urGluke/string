#include "s21_string.h"

char *s21_strncpy(char *dest, const char *src,
                  s21_size_t n)  //Копирует до n символов из строки, на которую
                                 //указывает src, в dest
{
  s21_size_t i = 0;
  for (; *(src + i) != '\0' && i < (s21_size_t)n; i++) {
    *(dest + i) = *(src + i);
  }
  for (; i < (s21_size_t)n; i++) {
    *(dest + i) = '\0';
  }
  return dest;
}
