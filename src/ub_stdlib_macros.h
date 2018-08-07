#ifndef UB_STDLIB_MACROS
#define UB_STDLIB_MACROS
#include "mini-printf/mini-printf.h"
#include "util.h"
void *ub_malloc(int);
void ub_free(void*);
#ifndef NULL
#define NULL (void*)0
#endif
#define malloc ub_malloc
#define free ub_free
#define printf ub_printf
#define memcpy ub_memcpy
#define putchar ub_putchar
#define strlen ub_strlen
#define strncmp ub_strncmp
#define strchr ub_strchr
#define atoi ub_atoi
#define isdigit ub_isdigit
#endif
