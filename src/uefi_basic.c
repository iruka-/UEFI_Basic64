#include <efi.h>
#include <efilib.h>
#include "mini-printf/mini-printf.h"
#include "util.h"
#include "efitable.h"
#include "ubasic/ubasic.h"
#define BUF_SIZE 1024
#define CBUF_SIZE 512
/* TODO: Add more comments */

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
	CHAR16 buf[BUF_SIZE];
	CHAR16 *bufptr = &buf[0];
	char cbuf[CBUF_SIZE];
	char linebuf[BUF_SIZE]; /* Program code memory */
	int i = 0;
	gST = eST;
	/*Initialize buffer memory*/
	for(i=0;i<BUF_SIZE;i++){
		buf[i] = 0;
	}
	/* Prompt */
	eST->ConOut->OutputString(eST->ConOut,L"UEFI BASIC v0.1\r\n");
	eST->ConOut->OutputString(eST->ConOut,L"Ok\r\n");
	ub_memset(linebuf,0,BUF_SIZE);
	/* Input loop */
	while(1){
		/* Wait for user to enter a line */
		ub_readline(eST,&bufptr);
		/* Read line into cbuf */
		ub_u2c(cbuf,buf,BUF_SIZE);
		/* If the line starts with a digit (if we are entering code into memory) */
		if(ub_isdigit(cbuf[0])){
			int i, digit, len, replace_flag;
			char digit_str[8];
			replace_flag = 0;
			len = ub_strlen(linebuf); /* Length of stored program code */
			digit = ub_atoi(cbuf); /* Line number of entered line */
			mini_snprintf(digit_str,5,"%d",digit);
			/* If entered line is greater than 2 chars and line starts with a digit*/
			if(len > 2 && ub_isdigit(linebuf[0])){
				int cur_digit = ub_atoi(linebuf);
				/* If we are replacing a line */
				if(cur_digit == digit){
					replace_flag = 1;
					int orig_line_len;
					char backup_linebuf[BUF_SIZE];
					/* Get length of the original line (the one we are replacing) */
					for(orig_line_len=0;linebuf[orig_line_len] != '\n';orig_line_len++);
					ub_memcpy(backup_linebuf,&linebuf[orig_line_len+1],BUF_SIZE);
					/* skip the next for loop to not look for a line number match again */
					len = 0;
					/* use snprintf() to add the new string into the program */
					mini_snprintf(linebuf,BUF_SIZE,"%s%s",cbuf,backup_linebuf);
				}
			}
			/* For each character in the program memory */
			/* Look for a matching line number to replace*/
			for(i=0;i<len;i++){
				/* If there is a newline and the next character is a digit */
				if(linebuf[i] == '\n' && ub_isdigit(linebuf[i+1])){
					int cur_digit = ub_atoi(&(linebuf[i+1]));
					/* If this is the right line we want to replace and the line numbers are the same, */
					/* replace the old line with the new line */
					if(cur_digit == digit){
						char truncated_linebuf[BUF_SIZE];
						char backup_linebuf[BUF_SIZE];
						int orig_line_start, end_line_start;
						int retval;
						replace_flag = 1;
						/* Set orig_line_start to the posistion of the start of the original line
						(the original line is the one that has the same line no as the one we're replacing)*/
						orig_line_start = i+1;
						/* Copy everything from the program memory up to the start of original line
						and store in truncated_linebuf*/
						retval = mini_snprintf(truncated_linebuf,orig_line_start+1,"%s",linebuf);
						/* Set end_line_start to the position of the end of the original line */
						for(end_line_start=orig_line_start;linebuf[end_line_start] != '\n';end_line_start++);
						/* What does this do?? */
						/* retval isn't used anywhere */
						for(retval=0;retval<30;retval++){
							ub_memcpy(backup_linebuf,linebuf,BUF_SIZE);
						}
						ub_memset(linebuf,0,BUF_SIZE); /*TODO: remove this*/
						/* Add new line into the program memory between the where the old line started and
						where the old line ended*/
						mini_snprintf(linebuf,BUF_SIZE,"%s%s%s",truncated_linebuf,cbuf,&backup_linebuf[end_line_start+1]);
					}
				}
			}
			/* Add code on to the end of the the program memory if we aren't replacing*/
			if(!replace_flag){
				mini_snprintf(linebuf,BUF_SIZE,"%s%s",linebuf,cbuf);
			}
		}
		/* Run the code on the BASIC interpeter */
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
		/* List out program */
		else if(ub_strncmp(cbuf,"list\r\n",7) == 0 || ub_strncmp(cbuf,"LIST\r\n",7) == 0){
			ub_printf("%s",linebuf);
			eST->ConOut->OutputString(eST->ConOut,L"Ok\r\n");
		}
		/* Clear out program code (create a new program) */
		else if(ub_strncmp(cbuf,"new\r\n",6) == 0 || ub_strncmp(cbuf,"NEW\r\n",6) == 0){
			ub_memset(linebuf,0,BUF_SIZE);
			eST->ConOut->OutputString(eST->ConOut,L"Ok\r\n");
		}
		else{
			eST->ConOut->OutputString(eST->ConOut,L"Syntax error\r\n");
		}
		ub_memset(buf,0,BUF_SIZE);
	}
	return EFI_SUCCESS;
}
