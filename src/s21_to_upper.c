#include "s21_string.h"

void *s21_to_upper(const char *str) {  // ГОТОВО! //
  char *str2 = s21_NULL;
  if (str != s21_NULL) {
    int t = 0;
    str2 = (char *)malloc((s21_strlen(str + 1)) * sizeof(char));
    if (!str2) exit(0);
    for (int i = 0; i < (int)s21_strlen(str); i++) {
      if (str[i] > 96 && str[i] < 123)
        str2[i] = str[i] - 32;
      else
        str2[i] = str[i];
      t++;
    }
    str2[t] = '\0';
  }
  return str2;
}