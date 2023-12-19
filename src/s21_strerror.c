#include "s21_string.h"

char *s21_strerror(int errnum) {
  static char result[100] = {0};

  if (errnum >= MIN_EER && errnum <= MAX_ERR) {
    s21_strcpy(result, s21_sys_errlist_apple[errnum]);
  } else {
    char str[100] = UNKNOWN;
    char errnum_str[100];
    s21_itoa(errnum, errnum_str);
    char *tmp = s21_strcat(str, errnum_str);
    s21_strcpy(result, tmp);
  }
  return result;
}
