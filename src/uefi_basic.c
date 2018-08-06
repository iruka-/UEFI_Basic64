#include <efi.h>
#include <efilib.h>
#include "mini-printf/mini-printf.h"
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
	ub_memset(linebuf,0,1024);	
	while(1){
		ub_readline(eST,&bufptr);
		ub_u2c(cbuf,buf,500);
		if(ub_isdigit(cbuf[0])){
			mini_snprintf(linebuf,1000,"%s%s",linebuf,cbuf);
		}
		else if(ub_strncmp(cbuf,"run\r\n",6) == 0 || ub_strncmp(cbuf,"RUN\r\n",6) == 0){
			ub_space(linebuf);
			ubasic_init(linebuf);
			do{
				ubasic_run();
			}
			while(!ubasic_finished());
			ub_unspace(linebuf);
			eST->ConOut->OutputString(eST->ConOut,L"Ok\r\n");
		}
		else if(ub_strncmp(cbuf,"list\r\n",7) == 0 || ub_strncmp(cbuf,"LIST\r\n",7) == 0){
			ub_printf("%s",linebuf);
			eST->ConOut->OutputString(eST->ConOut,L"Ok\r\n");
		}
		else if(ub_strncmp(cbuf,"new\r\n",6) == 0 || ub_strncmp(cbuf,"NEW\r\n",6) == 0){
			ub_memset(linebuf,0,1024);
			eST->ConOut->OutputString(eST->ConOut,L"Ok\r\n");
		}
		else{
			eST->ConOut->OutputString(eST->ConOut,L"Syntax error\r\n");
		}
		ub_memset(buf,0,512);
	}
	return EFI_SUCCESS;
}
