#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  char *token = s21_NULL;
  static char *olds = s21_NULL;
  if (str == s21_NULL) str = olds;
  if (str != s21_NULL) {
    str += s21_strspn(str, delim);
    if (*str == '\0') {
      olds = str;
      return s21_NULL;
    }
    token = str;
    str = s21_strpbrk(token, delim);
    if (str == s21_NULL) {
      olds = s21_memchr(token, '\0', 1024);
    } else {
      *str = '\0';
      olds = str + 1;
    }
  }
  return token;
}