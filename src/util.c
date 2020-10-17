#include <efi.h>
#include <efilib.h>
//#include "mini-printf/mini-printf.h"
#include "efitable.h"
#include "util.h"
int ub_c2u(CHAR16 *pwcs, const char *s, int n){
  int count = 0;
  if (n != 0) {
    do {
      if ((*pwcs++ = (CHAR16) *s++) == 0)
        break;
      count++;
    } while (--n != 0);
  }
  return count;
}

int ub_u2c (char *s,CHAR16 *pwcs,int n){
  int count = 0;
  
  if (n != 0) {
    do {
      if ((*s++ = (char) *pwcs++) == 0)
	break;
      count++;
    } while (--n != 0);
  }
  
  return count;
}

int ub_wstrcmp (CHAR16 *s1, CHAR16 *s2){ 
  while (*s1 != L'\0' && *s1 == *s2)
    {
      s1++;
      s2++;
    }

  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

void *ub_memset (void *m, int c, int n){
  char *s = (char *) m;
  while (n--)
    *s++ = (char) c;
  return m;
}

void *ub_memcpy (void *dst0, const void *src0, int len0){
  char *dst = (char *) dst0;
  char *src = (char *) src0;

  void *save = dst0;

  while (len0--)
    {
      *dst++ = *src++;
    }

  return save;
}

int ub_strncmp (const char *s1, const char *s2, int n){
  if (n == 0)
    return 0;

  while (n-- != 0 && *s1 == *s2)
    {
      if (n == 0 || *s1 == '\0')
	break;
      s1++;
      s2++;
    }

  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

char *ub_strchr (const char *s1, int i){
  const unsigned char *s = (const unsigned char *)s1;
  unsigned char c = i;
  while (*s && *s != c)
    s++;
  if (*s == c)
    return (char *)s;
  return NULL;
}

int ub_atoi(const char *s){
	int i,num=0,sign=1;
	int j;
	char t[15];
	ub_memcpy(t,s,10);
	for(j=0;s[j]<='0';j++)
		i=j;
	for(i=j;s[i];i++)
        {
                if(s[i]==' ')
                        continue;
                else if(s[i]=='-') sign=-1;
                else break;
        } 
        for(;s[i] && s[i]>='0' && s[i]<='9';i++)
                num=num*10 + s[i]-'0';
        return num*sign;
}

/*

int ub_printf(char *fmt,...){
	char buffer[1024];
	CHAR16 buffer16[1024];
	EFI_SYSTEM_TABLE *eST;
	int ret;
	va_list va;
	va_start(va,fmt);
	ret = mini_vsnprintf(buffer,1024,fmt,va);
	va_end(va);
	ub_c2u(buffer16,buffer,1024);
	eST = *ub_get_eST();
	eST->ConOut->OutputString(eST->ConOut,buffer16);
	return ret;
}

*/
int ub_putchar(int character, EFI_SYSTEM_TABLE *eST){
	CHAR16 ch[2]; /* This variable needs to be an array of 2 or else an extra space prints */
	ub_c2u(ch,(char *)&character,1);
	eST->ConOut->OutputString(eST->ConOut,&ch);
	return character;
}

unsigned int ub_strlen(const char *s){
	unsigned int len = 0;
	while (s[len] != '\0') len++;
	return len;
}

int ub_isdigit(char c){
	if ((c>='0') && (c<='9')) return 1;
   	return 0;
}

void ub_readline(EFI_SYSTEM_TABLE *eST,CHAR16 **buf){
	EFI_INPUT_KEY key;
	UINTN Index;
	CHAR16 *text = *buf;
	int Len = 0;
	while(1){
	  eST->BootServices->WaitForEvent(1, &(eST->ConIn->WaitForKey), &Index);
          eST->ConIn->ReadKeyStroke(eST->ConIn,&key);
          if (key.UnicodeChar == '\n' || key.UnicodeChar == '\r') {
            break;
          }
        
          if (key.UnicodeChar == '\b') {
            if (Len) {
                eST->ConOut->OutputString(eST->ConOut, L"\b \b");
                Len -= 1;
            }
            continue;
          }

         if (key.UnicodeChar >= '\x20') {
            text[Len] = key.UnicodeChar;

            text[Len+1] = 0;
            eST->ConOut->OutputString(eST->ConOut, &text[Len]);

            Len += 1;
            
            continue;
         }
    }
    eST->ConOut->OutputString(eST->ConOut,L"\r\n");
    text[Len] = L'\r';
    text[Len+1] = L'\n';
    text[Len+2] = 0;
}

/*
void ub_pause(){
	UINTN Index;
	EFI_SYSTEM_TABLE *eST = *ub_get_eST();
	eST->BootServices->WaitForEvent(1, &(eST->ConIn->WaitForKey), &Index);
}

*/
