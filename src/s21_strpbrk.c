#include "s21_string.h"

char *s21_strpbrk(
    const char *str1,
    const char *str2)  // Находит первый символ в строке str1, который
                       // соответствует любому символу, указанному в str2.
{
  s21_size_t i;
  char *find = s21_NULL;
  for (i = 0; *(str1 + i) && s21_strchr(str2, *(str1 + i)) == s21_NULL; i++)
    ;
  if (*(str1 + i)) {
    find = (char *)str1 + i;
  } else {
    find = s21_NULL;
  }
  return find;
}