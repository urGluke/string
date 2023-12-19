#include "s21_string.h"

s21_size_t s21_strlen(const char *str)  //Вычисляет длину строки str, не включая
                                        //завершающий нулевой символ
{
  s21_size_t length = 0;
  for (s21_size_t i = 0; str[i] != 0; i++) {
    length++;
  }
  return length;
}
