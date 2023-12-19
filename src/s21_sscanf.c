#include "s21_string.h"

typedef struct {
  int form;
  int supr;
  int width;
  int length;
  long int n;
  char buff;
  int eof;
  int error;
} flags;

int case_int(va_list args, flags *flag, char **strc, int sys);
int case_uns(va_list args, flags *flag, char **strc, int sys);
int case_flo(va_list args, flags *flag, char **strc);
int case_ptr(va_list args, flags *flag, char **strc);
int case_cha(va_list args, flags *flag, char **strc);
int case_str(va_list args, flags *flag, char **strc);
void space_trim(char **strc, flags *flag);
void case_per(char **strc, flags *flag, const char *format);
int flags_inv(char **strc, flags *flag, const char *format);
void case_n(va_list args, flags *flag);
void case_non_format(char **strc, flags *flag, const char *format);
int check_break(va_list args, flags *flag, char **strc, const char *format);
int switch_format(va_list args, flags *flag, char **strc, const char *format);
void numeric_buffer(char **strc, flags *flag);
long s21_strtol(char *str, char **strend, int sys);
unsigned long s21_strtoul(char *str, char **strend, int sys);
long double s21_strtoldd(char *str, char **strend);
float s21_strtof(char *str, char **strend);
double s21_strtod(char *str, char **strend);

int s21_sscanf(const char *str, const char *format, ...) {
  int res = 0;
  flags flag = {0, 0, 0, 0, 0, '\0', 0, 0};
  char *strc = malloc((s21_strlen(str) + 1) * sizeof(char));
  if (!strc) exit(0);
  strc = s21_strcpy(strc, str);
  space_trim(&strc, &flag);
  if (!*strc) flag.eof = 1;
  strc = s21_strcpy(strc - flag.n, str);
  flag.n = 0;
  va_list args;
  va_start(args, format);
  for (; *format; format++) {
    if (flags_inv(&strc, &flag, format)) continue;
    if (flag.form) {
      if ((int)*format > 47 && (int)*format < 58) {
        flag.width = flag.width * 10 + (int)*format - 48;
        continue;
      }
      res += switch_format(args, &flag, &strc, format);
    } else {
      case_non_format(&strc, &flag, format);
    }
    if (check_break(args, &flag, &strc, format)) {
      if (!res && flag.eof) res = -1;
      break;
    }
  }
  free(strc - flag.n);
  va_end(args);
  return res;
}

int case_int(va_list args, flags *flag, char **strc, int sys) {
  int res = 0;
  char *strend;
  numeric_buffer(strc, flag);
  if (s21_strtol(*strc, s21_NULL, sys) ||
      *(*strc + s21_strspn(*strc, "\n\t \f\x0B\r+-")) == '0') {
    if (!flag->supr) {
      res++;
      if (flag->length == 1)
        *(short int *)va_arg(args, void *) = s21_strtol(*strc, &strend, sys);
      else if (flag->length == 2)
        *(long int *)va_arg(args, void *) = s21_strtol(*strc, &strend, sys);
      else
        *(int *)va_arg(args, void *) = s21_strtol(*strc, &strend, sys);
    } else {
      s21_strtol(*strc, &strend, sys);
    }
    flag->n += strend - *strc;
    *strc = strend;
  } else {
    flag->error = 1;
  }
  flag->form = 0;
  return res;
}

int case_uns(va_list args, flags *flag, char **strc, int sys) {
  int res = 0;
  char *strend;
  numeric_buffer(strc, flag);
  if (s21_strtoul(*strc, s21_NULL, sys) ||
      *(*strc + s21_strspn(*strc, "\n\t \f\x0B\r+")) == '0') {
    if (!flag->supr) {
      if (s21_strtoul(*strc, s21_NULL, sys) || **strc == '0') res++;
      if (flag->length == 1)
        *(short unsigned *)va_arg(args, void *) =
            s21_strtoul(*strc, &strend, sys);
      else if (flag->length == 2)
        *(long unsigned *)va_arg(args, void *) =
            s21_strtoul(*strc, &strend, sys);
      else
        *(unsigned *)va_arg(args, void *) = s21_strtoul(*strc, &strend, sys);
    } else {
      s21_strtoul(*strc, &strend, sys);
    }
    flag->n += strend - *strc;
    *strc = strend;
  } else {
    flag->error = 1;
  }
  flag->form = 0;
  return res;
}

int case_flo(va_list args, flags *flag, char **strc) {
  int res = 0;
  char *strend;
  numeric_buffer(strc, flag);
  if (fabs(s21_strtod(*strc, s21_NULL)) >= 1e-100 ||
      isnan(s21_strtod(*strc, s21_NULL)) ||
      *(*strc + s21_strspn(*strc, "\n\t \f\x0B\r+-")) == '0') {
    if (!flag->supr) {
      res++;
      if (flag->length == 3)
        *(long double *)va_arg(args, void *) = s21_strtoldd(*strc, &strend);
      else if (flag->length == 2)
        *(double *)va_arg(args, void *) = s21_strtod(*strc, &strend);
      else
        *(float *)va_arg(args, void *) = s21_strtof(*strc, &strend);
    } else {
      s21_strtof(*strc, &strend);
    }
    flag->n += strend - *strc;
    *strc = strend;
  } else {
    flag->error = 1;
  }
  flag->form = 0;
  return res;
}

int case_ptr(va_list args, flags *flag, char **strc) {
  int res = 0;
  char *strend;
  if (s21_strtoul(*strc, s21_NULL, 16) ||
      *(*strc + s21_strspn(*strc, "\n\t \f\x0B\r+-")) == '0') {
    numeric_buffer(strc, flag);
    if (!flag->supr) {
      if (s21_strtoul(*strc, s21_NULL, 16) || **strc == '0') res++;
      *(void **)va_arg(args, void **) = (void *)s21_strtoul(*strc, &strend, 16);
    } else {
      s21_strtoul(*strc, &strend, 16);
    }
    flag->n += strend - *strc;
    *strc = strend;
  } else {
    flag->error = 1;
  }
  flag->form = 0;
  return res;
}

int case_cha(va_list args, flags *flag, char **strc) {
  int res = 0;
  if (!flag->width) flag->width = 2;
  if (!flag->supr) {
    res++;
    if (flag->length == 2)
      mbtowc((wchar_t *)va_arg(args, wchar_t *), *strc, flag->width);
    else
      *(char *)va_arg(args, char *) = **strc;
  }
  if (**strc < 0 && flag->length == 2) {
    (*strc)++;
    flag->n++;
  }
  (*strc)++;
  flag->n++;
  flag->form = 0;
  return res;
}

int case_str(va_list args, flags *flag, char **strc) {
  int res = 0;
  char *s = malloc(sizeof(char));
  if (!s) exit(0);
  space_trim(strc, flag);
  int i = 0;
  for (; **strc && **strc != ' ' && **strc != '\n' && **strc != '\t' &&
         **strc != '\r' && **strc != '\x0B' && **strc != '\f' &&
         (flag->width == 0 || i < flag->width);
       i++, (*strc)++) {
    s[i] = **strc;
    s = realloc(s, (i + 2) * sizeof(char));
    if (!s) exit(0);
  }
  s[i] = '\0';
  int k = s21_strlen(s) + 1;
  va_list s_list;
  va_copy(s_list, args);
  for (int j = 0; j < k; j++) {
    if (!flag->supr) {
      if (!flag->length) *((char *)va_arg(s_list, char *) + j) = s[j];
      if (j + 1 < k) {
        va_end(s_list);
        va_copy(s_list, args);
      }
    }
  }
  va_end(args);
  va_copy(args, s_list);
  if (flag->length == 2 && !flag->supr)
    mbstowcs((wchar_t *)va_arg(args, wchar_t *), s, k);
  free(s);
  if (!flag->supr) res++;
  flag->n += k - 1;
  flag->form = 0;
  va_end(s_list);
  return res;
}

void space_trim(char **strc, flags *flag) {
  while (**strc == ' ' || **strc == '\n' || **strc == '\t' || **strc == '\r' ||
         **strc == '\x0B' || **strc == '\f') {
    (*strc)++;
    flag->n++;
  }
}

void case_per(char **strc, flags *flag, const char *format) {
  space_trim(strc, flag);
  if (*format == **strc) {
    flag->n++;
    (*strc)++;
  } else {
    flag->error = 1;
  }
  flag->form = 0;
}

int flags_inv(char **strc, flags *flag, const char *format) {
  int res = 0;
  if (*format == '%' && !flag->form) {
    res++;
    flag->form = 1;
    flag->supr = 0;
    flag->length = 0;
    if (flag->buff) {
      *(*strc + s21_strlen(*strc)) = flag->buff;
      flag->buff = '\0';
    }
    flag->width = 0;
  }
  return res;
}

void case_n(va_list args, flags *flag) {
  if (!flag->supr) {
    if (flag->length == 1)
      *(short int *)va_arg(args, void *) = flag->n;
    else if (flag->length == 2)
      *(long int *)va_arg(args, void *) = flag->n;
    else
      *(int *)va_arg(args, void *) = flag->n;
  }
  flag->form = 0;
}

void case_non_format(char **strc, flags *flag, const char *format) {
  if (*format == ' ' || *format == '\t' || *format == '\n' || *format == '\r' ||
      *format == '\x0B') {
    while (*format == **strc) {
      (*strc)++;
      flag->n++;
    }
  } else if (**strc == *format) {
    (*strc)++;
    flag->n++;
  } else {
    flag->error = 1;
  }
}

int check_break(va_list args, flags *flag, char **strc, const char *format) {
  int res = 0;
  va_list backup;
  va_copy(backup, args);
  if ((!**strc && !flag->buff &&
       (s21_strstr(format, "%n") != s21_strchr(format, '%') ||
        !s21_strstr(format, "%n"))) ||
      !va_arg(backup, void *) || flag->error) {
    res = 1;
  }
  va_end(backup);
  return res;
}

int switch_format(va_list args, flags *flag, char **strc, const char *format) {
  int res = 0;
  switch (*format) {
    case 'x':
    case 'X':
      res += case_uns(args, flag, strc, 16);
      break;
    case 'p':
      res += case_ptr(args, flag, strc);
      break;
    case 'e':
    case 'E':
    case 'f':
    case 'g':
    case 'G':
      res += case_flo(args, flag, strc);
      break;
    case 'c':
      res += case_cha(args, flag, strc);
      break;
    case 'd':
      res += case_int(args, flag, strc, 10);
      break;
    case 'i':
      res += case_int(args, flag, strc, 0);
      break;
    case 'o':
      res += case_uns(args, flag, strc, 8);
      break;
    case 's': {
      res += case_str(args, flag, strc);
      break;
    }
    case 'u':
      res += case_uns(args, flag, strc, 10);
      break;
    case 'n':
      case_n(args, flag);
      break;
    case '%':
      case_per(strc, flag, format);
      break;
    case '*':
      flag->supr = 1;
      break;
    case 'h':
      flag->length = 1;
      break;
    case 'l':
      flag->length = 2;
      break;
    case 'L':
      flag->length = 3;
      break;
  }
  return res;
}

void numeric_buffer(char **strc, flags *flag) {
  space_trim(strc, flag);
  if (flag->width && !flag->buff && flag->width < (int)s21_strlen(*strc)) {
    flag->buff = *(*strc + flag->width);
    *(*strc + flag->width) = '\0';
  }
}

long s21_strtol(char *str, char **strend, int sys) {
  char *start = str;
  long int res = 0;
  int minus = 1;
  while (*str == ' ' || *str == '\n' || *str == '\t' || *str == '\r' ||
         *str == '\x0B' || *str == '\f')
    str++;
  if (*str == '+') str++;
  if (*str == '-') {
    minus = -1;
    str++;
  }
  if (sys == 0 && *str == '0' && *(str + 1) == 'x')
    sys = 16;
  else if (sys == 0 && *str == '0')
    sys = 8;
  else if (sys == 0)
    sys = 10;
  if (sys == 16 && *str == '0' && *(str + 1) == 'x') str += 2;
  if (sys == 10) {
    for (; *str > 47 && *str < 58; str++, res *= 10) res += *str - 48;
    res /= 10;
  } else if (sys == 8) {
    for (; *str > 47 && *str < 56; str++, res *= 8) res += *str - 48;
    res /= 8;
  } else if (sys == 16) {
    for (; (*str > 47 && *str < 58) || (*str > 64 && *str < 71) ||
           (*str > 96 && *str < 103);
         str++, res *= 16) {
      if (*str > 47 && *str < 58)
        res += *str - 48;
      else if ((*str > 64 && *str < 71))
        res += +*str - 55;
      else
        res += +*str - 87;
    }
    res /= 16;
  }
  if (strend) *strend = start + (str - start);
  return minus * res;
}

unsigned long s21_strtoul(char *str, char **strend, int sys) {
  return s21_strtol(str, strend, sys);
}

float s21_strtof(char *str, char **strend) {
  char *start = str;
  float a, decplace, b;
  int sign = 1;
  while (*str == ' ' || *str == '\n' || *str == '\t' || *str == '\r' ||
         *str == '\x0B' || *str == '\f')
    ++str;
  if (*str == '+') ++str;
  if (*str == '-') {
    sign = -1;
    ++str;
  }
  for (a = 0; *str && isdigit(*str); ++str) a = a * 10 + (*str - '0');
  if (*str == '.') ++str;
  for (decplace = 1.; *str && isdigit(*str); ++str, decplace *= 10.)
    a = a * 10. + (*str - '0');
  if (*str == 'e' || *str == 'E') {
    int bsign = 1;
    if (a <= 1e-6) a = 1;
    ++str;
    if (*str == '-') {
      bsign = -1;
      ++str;
    }
    if (*str == '+') ++str;
    for (b = 0; *str && isdigit(*str); ++str) b = b * 10 + (*str - '0');
    b *= bsign;
  } else {
    b = 0;
  }
  if ((*str == 'n' || *str == 'N') && (str[1] == 'a' || str[1] == 'A') &&
      (str[2] == 'n' || str[2] == 'N')) {
    str += 3;
    a = NAN * sign;
  }
  if ((*str == 'i' || *str == 'I') && (str[1] == 'n' || str[1] == 'N') &&
      (str[2] == 'f' || str[2] == 'F')) {
    str += 3;
    a = INFINITY * sign;
  }
  if (strend) *strend = start + (str - start);
  return (isnan(a) || isinf(a)) ? a : (a * sign / decplace) * powl(10, b);
}

double s21_strtod(char *str, char **strend) {
  char *start = str;
  double a, decplace, b;
  int sign = 1;
  while (*str == ' ' || *str == '\n' || *str == '\t' || *str == '\r' ||
         *str == '\x0B' || *str == '\f')
    ++str;
  if (*str == '+') ++str;
  if (*str == '-') {
    sign = -1;
    ++str;
  }
  for (a = 0; *str && isdigit(*str); ++str) a = a * 10 + (*str - '0');
  if (*str == '.') ++str;
  for (decplace = 1.; *str && isdigit(*str); ++str, decplace *= 10.)
    a = a * 10. + (*str - '0');
  if (*str == 'e' || *str == 'E') {
    int bsign = 1;
    if (a <= 1e-6) a = 1;
    ++str;
    if (*str == '-') {
      bsign = -1;
      ++str;
    }
    if (*str == '+') ++str;
    for (b = 0; *str && isdigit(*str); ++str) b = b * 10 + (*str - '0');
    b *= bsign;
  } else {
    b = 0;
  }
  if ((*str == 'n' || *str == 'N') && (str[1] == 'a' || str[1] == 'A') &&
      (str[2] == 'n' || str[2] == 'N')) {
    str += 3;
    a = NAN * sign;
  }
  if ((*str == 'i' || *str == 'I') && (str[1] == 'n' || str[1] == 'N') &&
      (str[2] == 'f' || str[2] == 'F')) {
    str += 3;
    a = INFINITY * sign;
  }
  if (strend) *strend = start + (str - start);
  return (isnan(a) || isinf(a)) ? a : (a * sign / decplace) * powl(10, b);
}

long double s21_strtoldd(char *str, char **strend) {
  char *start = str;
  long double a, decplace, b;
  int sign = 1;
  while (*str == ' ' || *str == '\n' || *str == '\t' || *str == '\r' ||
         *str == '\x0B' || *str == '\f')
    ++str;
  if (*str == '+') ++str;
  if (*str == '-') {
    sign = -1;
    ++str;
  }
  for (a = 0; *str && isdigit(*str); ++str) a = a * 10 + (*str - '0');
  if (*str == '.') ++str;
  for (decplace = 1.; *str && isdigit(*str); ++str, decplace *= 10.)
    a = a * 10. + (*str - '0');
  if (*str == 'e' || *str == 'E') {
    int bsign = 1;
    if (a <= 1e-6) a = 1;
    ++str;
    if (*str == '-') {
      bsign = -1;
      ++str;
    }
    if (*str == '+') ++str;
    for (b = 0; *str && isdigit(*str); ++str) b = b * 10 + (*str - '0');
    b *= bsign;
  } else {
    b = 0;
  }
  if ((*str == 'n' || *str == 'N') && (str[1] == 'a' || str[1] == 'A') &&
      (str[2] == 'n' || str[2] == 'N')) {
    str += 3;
    a = NAN * sign;
  }
  if ((*str == 'i' || *str == 'I') && (str[1] == 'n' || str[1] == 'N') &&
      (str[2] == 'f' || str[2] == 'F')) {
    str += 3;
    a = INFINITY * sign;
  }
  if (strend) *strend = start + (str - start);
  return (isnan(a) || isinf(a)) ? a : (a * sign / decplace) * powl(10, b);
}
