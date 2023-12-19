#include "s21_string.h"

void *s21_insert(const char *src, const char *str,
                 s21_size_t start_index) {  //ГОТОВО!//
  int t = 0;
  if (str != s21_NULL) t = (int)s21_strlen(str);
  int h = 0;
  if (src != s21_NULL) h = (int)s21_strlen(src);
  int k = 0;
  int l = 0;
  if ((int)start_index < 0) return s21_NULL;
  if (src != s21_NULL && str != s21_NULL)
    if (t == 0 && h == 0) {
      if (start_index != 0) return s21_NULL;
    }
  char *str1 = s21_NULL;
  if (src != s21_NULL) {
    if (str != s21_NULL)
      if (t == 0 && h == 0) {
        if (start_index == 0) str1 = (char *)calloc((1), sizeof(char));
        s21_strcat(str1, "");
      }
    if (t + h != 0 && t + h > (int)start_index) {
      str1 = (char *)calloc((t + h + 1), sizeof(char));
      char *str2 = (char *)calloc((t + h + 1), sizeof(char));
      for (int i = 0; i < (int)s21_strlen(src) + 1; i++) {
        if (i == start_index) {
          s21_strcat(str1, str);
          k++;
        } else {
          if (k == 0)
            str1[i] = src[i];
          else {
            str2[l] = src[i - 1];
            l++;
          }
        }
      }
      s21_strcat(str1, str2);
      if (str2) free(str2);
    }
  }
  return str1;
}
