# UEFI Basic
A simple BASIC language intepreter that runs on UEFI. Feel free to submit a pull request.
## Requirements
The gcc compiler for x86\_64 mingw platform (x86\_64-w64-mingw32-gcc, this can be found in the Debian package mingw-w64).
## Compilation
Running `make` will produce an EFI file and a bootable ISO for the x86\_64 architechture.
## Usage
Programs need to be typed in with line numbers. To run use the `run` command, and to create a new program use the `new` command. `list` prints out the current program.
## Credits
Credit to [gnu-efi](https://github.com/vathpela/gnu-efi), [ubasic](https://github.com/adamdunkels/ubasic), [mini-printf](https://github.com/mludvig/mini-printf) and to [newlib](https://github.com/bminor/newlib).
## Downloads
UEFI bootable ISO files can be found on the [releases page](https://github.com/logern5/UEFI_Basic/releases).
## TODO:
Running the efi file sometimes gives a load error. I can run it and rerun it, but after resetting the VM, I get "Command Error Status: Load error". 
The md5sum seems to be different before starting the VM and after closing it. Creating a backup file seems to fix it. The error occurs only after a hard reset
or shutdown.

The conversion from 8 to 16 bit chars, along with passing the *eST pointer with every outchar() causes overhead (change to global var)

Remove commented out lines

Remove now unneeded functions and files

Add PEEK/POKE support for pointers, arrays, and strings. Add 8-bit PEEK/POKES as well as possibly 16 bit. POKE and PEEK both work (Peek needs parantheses with spacing,
such as PEEK( 1000 ), same with the ABS() function). I added the actual POKE functionality. Ocassionally when I poke I get a "How?" message.
