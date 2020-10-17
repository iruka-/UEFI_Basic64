#ifndef ub_util
#define ub_util
#include <efi.h>
#include <efilib.h>
int ub_c2u(CHAR16 *pwcs, const char *s, int n);
int ub_u2c (char *s,CHAR16 *pwcs,int n);
int ub_wstrcmp (CHAR16 *s1, CHAR16 *s2);
void *ub_memset (void *m, int c, int n);
void *ub_memcpy (void *dst0, const void *src0, int len0);
int ub_strncmp (const char *s1, const char *s2, int n);
char *ub_strchr (const char *s1, int i);
int ub_atoi(const char *str);
//int ub_printf(char *fmt,...);
int ub_putchar(int character, EFI_SYSTEM_TABLE *eST);
unsigned int ub_strlen(const char *s);
void ub_readline(EFI_SYSTEM_TABLE *eST,CHAR16 **buf);
int ub_isdigit(char c);
void ub_pause(void);
#endif
