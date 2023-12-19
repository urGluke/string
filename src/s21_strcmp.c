#include "s21_string.h"

int s21_strcmp(
    const char *str1,
    const char *str2)  //Сравнивает строку, на которую указывает
                       // str1, со строкой, на которую указывает str2.
{
  int flag = 0;
  for (s21_size_t i = 0; *(str1 + i) != '\0' || *(str2 + i) != '\0'; i++) {
    if (*(str1 + i) != *(str2 + i)) {
      flag = *(str1 + i) - *(str2 + i);
      break;
    }
  }
  return flag;
}
