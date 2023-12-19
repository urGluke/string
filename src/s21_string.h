#include <ctype.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define BUFF_SIZE 4096
#define s21_NULL (void *)0
#define TOKENS "cdieEfgGosuxXpn"

typedef unsigned long s21_size_t;
typedef long unsigned s21_size_t;
typedef enum token_width { NONE_WIDTH, WIDTH_NUMBER, WIDTH_AST } token_width;


typedef enum token_length {
  NONE_LENGTH,
  LENGTH_SHORT,
  LENGTH_LONG,
  LENGTH_LONG_LONG,
  LENGTH_DOUBLE,
  LENGTH_LONG_DOUBLE
} token_length;

typedef struct token {
  // %[width][.precision][length][specifier]
  void *address;
  token_width width;
  unsigned int width_status;
  token_length token_length;
  char buff[1024];
  char spec;
} token;

typedef union {
  float f;
  struct {
    unsigned int mantisa : 23;
    unsigned int exponent : 8;
    unsigned int sign : 1;
  } parts;
} float_cast;

typedef unsigned long s21_size_t;

typedef struct var_parameters {
  int align_minus;
  int show_sign;
  int space_for_pos;
  int hash_zero;
  int direct_zero;
  int width;
  int star_width;
  int star_precision;
  int precision_zero;
  int dot_check;
  int precision_num;
  int len_h;
  int len_l;
  int len_double;
} params;

void process_string(va_list ap, params *variable, char *s, int *top);
void process_x(va_list ap, params *variable, long unsigned int v_x, char *array,
               int *top, const char *format);
void process_g(va_list ap, params *variable, long double v_e, char *array,
               int *top, const char *format_pointer);
void process_float(va_list ap, params *variable, long double v_f, char *array,
                   int *top);
void process_exp(va_list ap, params *variable, long double v_e, char *array,
                 int *top, const char *format_pointer);
void process_char(va_list ap, params *variable, char *array, int *top);
void process_int(va_list ap, params *variable, char *array, int *top,
                 const char *gen_pointer);
void process_oct(va_list ap, params *variable, char *array, int *top);
void process_p(va_list ap, params *variable, int *v_p, char *array, int *top);
void write_into_main_buf(const char *temp, char *main_buf, int *top, int count,
                         int flag, int hex_flag, int sign);
void align_left(char *main_buf, int *top, int len, int fill, params *variable,
                const char *temp);
int add_zero(char **temp, char *main_buf, int *top, int len, params *variable,
             int *shift);
void add_space(const char *temp, char *main_buf, int *top, int len,
               params *variable);
int parse_params(const char *str_pointer, params *variable, int *shift);
void init_struct(params *res);
char *s21_itoaa(long int num);
char *oct_to_string(long int num, params *variable);
void delete_zero(char *g_string, int *top);
char *hex_to_string(unsigned long int num, int upper_flag, int pointer_flag,
                    int nil_flag);
char *float_to_string(long double num, int g_flag, params *variable);
char *exp_to_string(long double v_e, const char *exp_letter, int g_flag,
                    params *variable);
int check_fraction(int *top, params *variable, long double num);
int check_precision(params *variable, int expo);
int add_precision_zero(char **temp, char *main_buf, int *top, params *variable,
                       int flag_x, int *shift);
void add_space_precision_zero(char *temp, char *main_buf, int *top,
                              params *variable);
int find_exp(long double num);
void long_char_helper(wchar_t *long_str, params *variable, char *s, int *top);
void null_helper(params *variable, char *s, int *top);
void normal_string_helper(char *simple_str, params *variable, char *s,
                          int *top);

int s21_sprintf(char *s, const char *format, ...);

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memmove(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strcat(char *dest, const char *src);
char *s21_strncat(char *dest, const char *src, size_t n);
s21_size_t s21_strlen(const char *str);
int s21_strcmp(const char *str1, const char *str2);
char *s21_strcpy(char *dest, const char *src);
char *s21_strchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
size_t s21_strspn(const char *str1, const char *str2);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strtok(char *str, const char *delim);

int s21_isdigit(char c);
int s21_atoi(const char *str_carriage);
int s21_sscanf(const char *str, const char *format, ...);

s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
char *s21_wstrncat(char *dest, const wchar_t *src, s21_size_t n);
char *s21_strrchr(const char *str, int c);
char *s21_strerror(int errnum);
char *s21_itoa(long int num, char *str);
s21_size_t s21_strncmp(const char *str1, const char *str2, s21_size_t n);

void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);
int find_ind(const char *src, const char *trim_chars, int index);

#if __APPLE__

#define UNKNOWN "Unknown error: "
#define MIN_EER 0
#define MAX_ERR 106

#define s21_sys_errlist_apple                                    \
  ((char *[]){"Undefined error: 0",                              \
              "Operation not permitted",                         \
              "No such file or directory",                       \
              "No such process",                                 \
              "Interrupted system call",                         \
              "Input/output error",                              \
              "Device not configured",                           \
              "Argument list too long",                          \
              "Exec format error",                               \
              "Bad file descriptor",                             \
              "No child processes",                              \
              "Resource deadlock avoided",                       \
              "Cannot allocate memory",                          \
              "Permission denied",                               \
              "Bad address",                                     \
              "Block device required",                           \
              "Resource busy",                                   \
              "File exists",                                     \
              "Cross-device link",                               \
              "Operation not supported by device",               \
              "Not a directory",                                 \
              "Is a directory",                                  \
              "Invalid argument",                                \
              "Too many open files in system",                   \
              "Too many open files",                             \
              "Inappropriate ioctl for device",                  \
              "Text file busy",                                  \
              "File too large",                                  \
              "No space left on device",                         \
              "Illegal seek",                                    \
              "Read-only file system",                           \
              "Too many links",                                  \
              "Broken pipe",                                     \
              "Numerical argument out of domain",                \
              "Result too large",                                \
              "Resource temporarily unavailable",                \
              "Operation now in progress",                       \
              "Operation already in progress",                   \
              "Socket operation on non-socket",                  \
              "Destination address required",                    \
              "Message too long",                                \
              "Protocol wrong type for socket",                  \
              "Protocol not available",                          \
              "Protocol not supported",                          \
              "Socket type not supported",                       \
              "Operation not supported",                         \
              "Protocol family not supported",                   \
              "Address family not supported by protocol family", \
              "Address already in use",                          \
              "Can't assign requested address",                  \
              "Network is down",                                 \
              "Network is unreachable",                          \
              "Network dropped connection on reset",             \
              "Software caused connection abort",                \
              "Connection reset by peer",                        \
              "No buffer space available",                       \
              "Socket is already connected",                     \
              "Socket is not connected",                         \
              "Can't send after socket shutdown",                \
              "Too many references: can't splice",               \
              "Operation timed out",                             \
              "Connection refused",                              \
              "Too many levels of symbolic links",               \
              "File name too long",                              \
              "Host is down",                                    \
              "No route to host",                                \
              "Directory not empty",                             \
              "Too many processes",                              \
              "Too many users",                                  \
              "Disc quota exceeded",                             \
              "Stale NFS file handle",                           \
              "Too many levels of remote in path",               \
              "RPC struct is bad",                               \
              "RPC version wrong",                               \
              "RPC prog. not avail",                             \
              "Program version wrong",                           \
              "Bad procedure for program",                       \
              "No locks available",                              \
              "Function not implemented",                        \
              "Inappropriate file type or format",               \
              "Authentication error",                            \
              "Need authenticator",                              \
              "Device power is off",                             \
              "Device error",                                    \
              "Value too large to be stored in data type",       \
              "Bad executable (or shared library)",              \
              "Bad CPU type in executable",                      \
              "Shared library version mismatch",                 \
              "Malformed Mach-o file",                           \
              "Operation canceled",                              \
              "Identifier removed",                              \
              "No message of desired type",                      \
              "Illegal byte sequence",                           \
              "Attribute not found",                             \
              "Bad message",                                     \
              "EMULTIHOP (Reserved)",                            \
              "No message available on STREAM",                  \
              "ENOLINK (Reserved)",                              \
              "No STREAM resources",                             \
              "Not a STREAM",                                    \
              "Protocol error",                                  \
              "STREAM ioctl timeout",                            \
              "Operation not supported on socket",               \
              "Policy not found",                                \
              "State not recoverable",                           \
              "Previous owner died",                             \
              "Interface output queue is full"})

#elif __linux__

#define UNKNOWN "Unknown error: "
#define MIN_EER 0
#define MAX_ERR 133

#define s21_sys_errlist_apple                                      \
  ((char *[]){"Success",                                           \
              "Operation not permitted",                           \
              "No such file or directory",                         \
              "No such process",                                   \
              "Interrupted system call",                           \
              "Input/output error",                                \
              "No such device or address",                         \
              "Argument list too long",                            \
              "Exec format error",                                 \
              "Bad file descriptor",                               \
              "No child processes",                                \
              "Resource temporarily unavailable",                  \
              "Cannot allocate memory",                            \
              "Permission denied",                                 \
              "Bad address",                                       \
              "Block device required",                             \
              "Device or resource busy",                           \
              "File exists",                                       \
              "Invalid cross-device link",                         \
              "No such device",                                    \
              "Not a directory",                                   \
              "Is a directory",                                    \
              "Invalid argument",                                  \
              "Too many open files in system",                     \
              "Too many open files",                               \
              "Inappropriate ioctl for device",                    \
              "Text file busy",                                    \
              "File too large",                                    \
              "No space left on device",                           \
              "Illegal seek",                                      \
              "Read-only file system",                             \
              "Too many links",                                    \
              "Broken pipe",                                       \
              "Numerical argument out of domain",                  \
              "Numerical result out of range",                     \
              "Resource deadlock avoided",                         \
              "File name too long",                                \
              "No locks available",                                \
              "Function not implemented",                          \
              "Directory not empty",                               \
              "Too many levels of symbolic links",                 \
              "Unknown error 41",                                  \
              "No message of desired type",                        \
              "Identifier removed",                                \
              "Channel number out of range",                       \
              "Level 2 not synchronized",                          \
              "Level 3 halted",                                    \
              "Level 3 reset",                                     \
              "Link number out of range",                          \
              "Protocol driver not attached",                      \
              "No CSI structure available",                        \
              "Level 2 halted",                                    \
              "Invalid exchange",                                  \
              "Invalid request descriptor",                        \
              "Exchange full",                                     \
              "No anode",                                          \
              "Invalid request code",                              \
              "Invalid slot",                                      \
              "Unknown error 58",                                  \
              "Bad font file format",                              \
              "Device not a stream",                               \
              "No data available",                                 \
              "Timer expired",                                     \
              "Out of streams resources",                          \
              "Machine is not on the network",                     \
              "Package not installed",                             \
              "Object is remote",                                  \
              "Link has been severed",                             \
              "Advertise error",                                   \
              "Srmount error",                                     \
              "Communication error on send",                       \
              "Protocol error",                                    \
              "Multihop attempted",                                \
              "RFS specific error",                                \
              "Bad message",                                       \
              "Value too large for defined data type",             \
              "Name not unique on network",                        \
              "File descriptor in bad state",                      \
              "Remote address changed",                            \
              "Can not access a needed shared library",            \
              "Accessing a corrupted shared library",              \
              ".lib section in a.out corrupted",                   \
              "Attempting to link in too many shared libraries",   \
              "Cannot exec a shared library directly",             \
              "Invalid or incomplete multibyte or wide character", \
              "Interrupted system call should be restarted",       \
              "Streams pipe error",                                \
              "Too many users",                                    \
              "Socket operation on non-socket",                    \
              "Destination address required",                      \
              "Message too long",                                  \
              "Protocol wrong type for socket",                    \
              "Protocol not available",                            \
              "Protocol not supported",                            \
              "Socket type not supported",                         \
              "Operation not supported",                           \
              "Protocol family not supported",                     \
              "Address family not supported by protocol",          \
              "Address already in use",                            \
              "Cannot assign requested address",                   \
              "Network is down",                                   \
              "Network is unreachable",                            \
              "Network dropped connection on reset",               \
              "Software caused connection abort",                  \
              "Connection reset by peer",                          \
              "No buffer space available",                         \
              "Transport endpoint is already connected",           \
              "Transport endpoint is not connected",               \
              "Cannot send after transport endpoint shutdown",     \
              "Too many references: cannot splice",                \
              "Connection timed out",                              \
              "Connection refused",                                \
              "Host is down",                                      \
              "No route to host",                                  \
              "Operation already in progress",                     \
              "Operation now in progress",                         \
              "Stale file handle",                                 \
              "Structure needs cleaning",                          \
              "Not a XENIX named type file",                       \
              "No XENIX semaphores available",                     \
              "Is a named type file",                              \
              "Remote I/O error",                                  \
              "Disk quota exceeded",                               \
              "No medium found",                                   \
              "Wrong medium type",                                 \
              "Operation canceled",                                \
              "Required key not available",                        \
              "Key has expired",                                   \
              "Key has been revoked",                              \
              "Key was rejected by service",                       \
              "Owner died",                                        \
              "State not recoverable",                             \
              "Operation not possible due to RF-kill",             \
              "Memory page has hardware error"})

#endif
char *s21_strerror(int errnum);

/* СТРУКТУРА:
 * флаг(плюс/минус)
 * ширина - width (число перед спецификатором)
 * точность - precision (точка)
 * длина - length ()
 * спецификатор(буква после ширины)
 */

token token_parse(char **format, va_list *ap);
int space_check(char c);
int register_check(char c);

long double s21_strtold(const char *nptr, char **endptr);
unsigned long long int s21_strntollu(const char *string, char **endptr,
                                     int basis, int n_byte);

void skip_chars_in_buffer(char **src_carriage, int *fail, token *t);
void skip_spaces_in_str(char **src);

int check_EOF_string(const char *src);

void format_parse_width(char **format, token *t);
void format_parse_length(char **format, token *t);
void format_parse_specifier(char **format, token *t);

int parse_number_from_format(char **format);

void write_tokens_to_memory(char **str_carriage, token *t, int token_carriage,
                            int *res);
void write_char_to_memory(char **str_carriage, int *res, token *t);
void write_int_to_memory(char **str_carriage, int *fail_flag, int *res,
                         token *t);
void write_float_to_memory(char **str_carriage, int *res, token *t);
void write_string_to_memory(char **str_carriage, const int *fail_flag, int *res,
                            token *t);
void write_unsigned_to_memory(char **str_carriage, int *fail_flag, int *res,
                              token *t);
void write_8_16_to_memory(char **str_carriage, int *fail_flag, int *res,
                          token *t, int base);
void write_unspec_int_to_memory(char **str_carriage, int *fail_flag, int *res,
                                token *t);

void unsigned_type_converter(token *t, unsigned long long int result, int sign);
void int_type_converter(token *t, long long int result, int sign);
void float_type_converter(token *t, long double result);
