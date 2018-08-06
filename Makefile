CC=x86_64-w64-mingw32-gcc
CFLAGS=-ffreestanding -I./gnu-efi/inc -I./gnu-efi/inc/x86_64 -I./gnu-efi/inc/protocol -Wall -Wextra --std=c99 -pedantic
ISO_FILE = uefi_basic.iso
all: build iso
build:
	$(CC) $(CFLAGS) -c -o mini-printf.o ./src/mini-printf/mini-printf.c

	$(CC) $(CFLAGS) -c -o uefi_basic.o ./src/uefi_basic.c
	
	$(CC) $(CFLAGS) -c -o util.o ./src/util.c

	$(CC) $(CFLAGS) -c -o ubasic.o ./src/ubasic/ubasic.c
	$(CC) $(CFLAGS) -c -o tokenizer.o ./src/ubasic/tokenizer.c

	$(CC) $(CFLAGS) -c -o data.o ./gnu-efi/lib/data.c
	$(CC) -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main -o BOOTX64.EFI mini-printf.o ubasic.o tokenizer.o uefi_basic.o util.o data.o -lgcc
iso:
	mkisofs -o uefi_basic.iso --root /EFI/BOOT BOOTX64.EFI
clean:
	rm *.iso *.o *.EFI
