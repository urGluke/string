#include "s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  int h = 0;
  char *str2 = s21_NULL;
  if (src != s21_NULL) {
    h = (int)s21_strlen(src);
    str2 = (char *)calloc(h, sizeof(char));
    s21_strcpy(str2, src);
    if (trim_chars != s21_NULL) {
      for (int i = 0; i < (int)s21_strlen(trim_chars); i++) {
        for (int j = 0; j < (int)s21_strlen(str2); j++) {
          while (str2[j] == trim_chars[i]) {
            for (int k = j; k < s21_strlen(str2); k++) str2[k] = str2[k + 1];
          }
        }
      }
    }
  }
  return str2;
}