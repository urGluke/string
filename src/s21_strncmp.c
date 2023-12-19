#include "s21_string.h"

s21_size_t s21_strncmp(
    const char *str1, const char *str2,
    s21_size_t n)  //Сравнивает не более первых n байтов str1 и str2
{
  int flag = 0;
  for (s21_size_t i = 0; (*(str1 + i) != '\0' || *(str2 + i) != '\0') && i < n;
       ++i) {
    if (*(str1 + i) != *(str2 + i)) {
      flag = *(str1 + i) - *(str2 + i);
      break;
    }
  }
  return flag;
}