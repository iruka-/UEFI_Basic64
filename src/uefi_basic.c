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
	CHAR16 buf[512];
	CHAR16 *bufptr = &buf[0];
	char cbuf[512];
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
			int i, digit, len, replace_flag;
			char digit_str[8];
			char cur_digit[8];
			replace_flag = 0;
			len = ub_strlen(linebuf);
			digit = ub_atoi(cbuf);
			//ub_printf("digit: %d\r\n",digit);
			mini_snprintf(digit_str,5,"%d",digit);
			//ub_printf("digit_str: %s\r\n",digit_str);
			if(len > 2 && ub_isdigit(linebuf[0])){
				int cur_digit = ub_atoi(linebuf);
				//ub_printf("linebuf[0] isdigit\r\n");
				if(cur_digit == digit){
					replace_flag = 1;
					int orig_line_len, growlen;
					char backup_linebuf[1024];
					for(orig_line_len=0;linebuf[orig_line_len] != '\n';orig_line_len++){
					}
					//ub_printf("cbuf len: %d\r\n",ub_strlen(cbuf));
					//ub_printf("linebuf len: %d\r\n",i);
					//growlen = ub_strlen(cbuf) - orig_line_len - 1;
					//ub_printf("match found at beginning of line\r\n");
					//ub_printf("linebuf text: %s\r\n", linebuf);
					//ub_printf("cbuf text: %s\r\n", cbuf);
					//ub_printf("growlen: %d\r\n", growlen);
					//ub_printf("orig_line_len: %d\r\n",orig_line_len);
					//ub_printf("linebuf[orig_line_len+1] = 0x%x,%c\r\n",linebuf[orig_line_len+1],linebuf[orig_line_len+1]);
					ub_memcpy(backup_linebuf,&linebuf[orig_line_len+1],1024);
					len = 0; /* skip the for loop to not look for a match again */
					/* use snprintf() to grow the string */
					//ub_printf("backup_linebuf:%s\r\n",backup_linebuf);
					mini_snprintf(linebuf,1024,"%s%s",cbuf,backup_linebuf);
					//ub_printf("new linebuf:%s\r\n",linebuf);
				}
			}
			for(i=0;i<len;i++){
				//ub_printf("linebuf[%d] == %c\r\n",i,linebuf[i]);
				if(linebuf[i] == '\n' && ub_isdigit(linebuf[i+1])){
					int cur_digit = ub_atoi(&(linebuf[i+1]));
					//ub_printf("linebuf digit: %d\r\n",digit);
					if(cur_digit == digit){
						char truncated_linebuf[1024];
						char backup_linebuf[1024];
						int orig_line_start, end_line_start;
						int retval;
						replace_flag = 1;
						//ub_printf("match found\r\n");
						//ub_printf("linebuf text: %s\r\n", &linebuf[i]);
						//ub_printf("cbuf text: %s\r\n", cbuf);
						orig_line_start = i+1;
						//ub_printf("orig_line_start: %d\r\n",orig_line_start);
						retval = mini_snprintf(truncated_linebuf,orig_line_start+1,"%s",linebuf);
						//ub_printf("mini_snprintf() retval: %d\r\n",retval);
						for(end_line_start=orig_line_start;linebuf[end_line_start] != '\n';end_line_start++){
						}
						//ub_printf("truncated_linebuf: %s\r\n",truncated_linebuf);
						for(retval=0;retval<30;retval++)
							//ub_printf("(%d,0x%x)",retval,truncated_linebuf[retval]);
						//ub_printf("end_line_start: %d\r\n",end_line_start);
						//ub_printf("&linebuf[end_line_start]: %s\r\n",&linebuf[end_line_start]);
						ub_memcpy(backup_linebuf,linebuf,1024);
						ub_memset(linebuf,0,1024); /*TODO: remove this*/
						//ub_printf("backup_linebuf: %s\r\n",backup_linebuf);
						mini_snprintf(linebuf,1024,"%s%s%s",truncated_linebuf,cbuf,&backup_linebuf[end_line_start+1]);
						//ub_printf("past snprintf()\r\n");
						//ub_printf("hexdump linebuf\r\n");
						//for(retval=0;retval<30;retval++)
							//ub_printf("(%d,0x%x)",retval,linebuf[retval]);
					}
				}
			}
			if(!replace_flag){
				mini_snprintf(linebuf,1000,"%s%s",linebuf,cbuf);
			}
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
