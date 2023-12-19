#include "s21_string.h"

s21_size_t s21_strspn(
    const char *str1,
    const char *str2)  // Вычисляет длину начального сегмента str1,
                       // который полностью состоит из символов str2.
{
  s21_size_t i;
  for (i = 0; str1[i] != '\0'; i++) {
    for (s21_size_t j = 0; str2[j] != str1[i]; j++) {
      if (str2[j] == '\0') {
        return i;
      }
    }
  }
  return i;
}