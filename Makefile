#
# M a k e f i l e
#
CC=x86_64-w64-mingw32-gcc
DESK_CC=gcc
CFLAGS=-ffreestanding -I./gnu-efi/inc -I./gnu-efi/inc/x86_64 -I./gnu-efi/inc/protocol -Wall --std=c99
MKISOFS=xorrisofs
QEMU="/mnt/c/Program Files/qemu/qemu-system-x86_64.exe"
QEMU="qemu-system-x86_64"

#all: uefi

all: desktop

uefi:
	$(CC) $(CFLAGS) -DFORCE_UEFI -c -o basic.o ./src/ubasic/basic.c
	$(CC) $(CFLAGS) -c -o util.o ./src/util.c
	$(CC) -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main -o BOOTX64.EFI util.o basic.o

desktop:
	$(DESK_CC) -Wall -g -DFORCE_DESKTOP -o basic ./src/ubasic/basic.c

iso:
	$(MKISOFS) -o uefi_basic.iso --root /EFI/BOOT BOOTX64.EFI

clean:
	rm -f *.iso *.o *.EFI basic *.efi *~ err

qemu:
	cp -n BOOTX64.EFI bak.efi
	$(QEMU) -bios ./bios64.bin -drive file=fat:rw:./,format=raw
#
