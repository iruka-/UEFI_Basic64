#include <efi.h>
#include <efilib.h>
#include "mini-printf/mini-printf.h"
#include "newlib/mbstowcs.c"
#include "util.h"
#include "efitable.h"
#include "ubasic/ubasic.h"

EFI_SYSTEM_TABLE *gST;

EFI_SYSTEM_TABLE **ub_get_eST(){
	return &gST;
}
void *ub_malloc(int bytes){
	void *mem;
	gST->BootServices->AllocatePool(EfiLoaderData,bytes,&mem);
	return mem;
}

void ub_free(void *ptr){
	gST->BootServices->FreePool(ptr);
}

void ub_space(char *str){
	int i;
	int len = ub_strlen(str);
	for(i=0;i<len;i++){
		if(str[i] == '\r' && str[i+1] == '\n'){
			str[i] = '\x20';
		}
	}
}
void ub_unspace(char *str){
	int i;
	int len = ub_strlen(str);
	for(i=0;i<len;i++){
		if(str[i] == '\x20' && str[i+1] == '\n'){
			str[i] = '\r';
		}
	}
}	
EFI_STATUS efi_main(EFI_HANDLE eIH, EFI_SYSTEM_TABLE *eST){
	UINTN Index;
	CHAR16 buf[512];
	CHAR16 *bufptr = &buf[0];
	char cbuf[512];
	char **cbufptr = &cbuf[0];
	char linebuf[1024];
	int i = 0;
	gST = eST;
	/*Initialize buffer memory*/
	for(i=0;i<50;i++){
		buf[i] = 0;
	}
	
	eST->ConOut->OutputString(eST->ConOut,L"UEFI BASIC v0.1\r\n");
	eST->ConOut->OutputString(eST->ConOut,L"Ok\r\n");
	/*begin tests*/
	/*
	eST->ConOut->OutputString(eST->ConOut,L"Testing ub_wstrcmp()\r\n");
	if(ub_wstrcmp(L"HELLO",L"HELLO") == 0){
		eST->ConOut->OutputString(eST->ConOut,L"ub_wstrcmp() ok\r\n");
	}
	
	eST->ConOut->OutputString(eST->ConOut,L"Testing ub_printf()\r\n");
	ub_printf("ub_printf() %s","ok\r\n");
	
	eST->ConOut->OutputString(eST->ConOut,L"Testing ub_putchar()\r\n");
	ub_putchar('Z');*/
	/*end tests*/

	/*ubasic tests*/
	//char *program ="10 gosub 70\x20\n20 for i = 1 to 10\x20\n30 print i\x20\n40 next i\x20\n50 print \"end\"\x20\n60 end\x20\n70 print \"subroutine\"\x20\n110 return\x20\n";
	//char *program = "10 for i = 1 to 10\n20 print i\n30 next i";
	//char *program = "10 x = 5\n20 if x = 5 goto 40\n30 print \"x isnt 5\"\n40 print \"x is 5\"";
	/*ubasic_init(program);
 	do {
 		ubasic_run();
 	} while(!ubasic_finished());*/
	/*end ubasic tests*/

	ub_memset(linebuf,0,1024);	
	while(1){
		ub_readline(eST,&bufptr);
		ub_u2c(cbuf,buf,500);
		//ub_space(cbuf);
		//eST->ConOut->OutputString(eST->ConOut,L"buf:");
		//eST->ConOut->OutputString(eST->ConOut,buf);
		//ub_printf("cbuf: '%s'\r\n",cbuf);
		if(ub_isdigit(cbuf[0])){
			mini_snprintf(linebuf,1000,"%s%s",linebuf,cbuf);
			//ub_printf("linebuf: '%s'\r\n",linebuf);
		}
		if(ub_strncmp(cbuf,"run\r\n",6) == 0 || ub_strncmp(cbuf,"RUN\r\n",6) == 0){
			//ub_printf("run command\r\n");
			ub_space(linebuf);
			//ub_printf("spaced linebuf: '%s'\r\n",linebuf);
			ubasic_init(linebuf);
			do{
				ubasic_run();
			}
			while(!ubasic_finished());
			ub_unspace(linebuf);
			ub_printf("Ok\r\n");
		}
		else if(ub_strncmp(cbuf,"list\r\n",7) == 0 || ub_strncmp(cbuf,"LIST\r\n",7) == 0){
			ub_printf("%s",linebuf);
			ub_printf("Ok\r\n");
		}
		else if(ub_strncmp(cbuf,"new\r\n",6) == 0 || ub_strncmp(cbuf,"NEW\r\n",6) == 0){
			ub_memset(linebuf,0,1024);
			ub_printf("Ok\r\n");
		}
		ub_memset(buf,0,512);
	}
	eST->ConOut->OutputString(eST->ConOut,L"something\r\n");
	
	eST->BootServices->WaitForEvent(1, &(eST->ConIn->WaitForKey), &Index);
	return EFI_SUCCESS;
}
