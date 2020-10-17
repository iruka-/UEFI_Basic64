# UEFI Basic
A simple BASIC language intepreter that runs on UEFI. Feel free to submit a pull request.
## Requirements
The gcc compiler for x64\_64 mingw platform (x86\_64-w64-mingw32-gcc).
## Compilation
Running `make` will produce an EFI file and a bootable ISO for the x86\_64 architechture.
## Usage
Programs need to be typed in with line numbers. To run use the `run` command, and to create a new program use the `new` command. `list` prints out the current program.
## Credits
Credit to [gnu-efi](https://github.com/vathpela/gnu-efi), [ubasic](https://github.com/adamdunkels/ubasic), [mini-printf](https://github.com/mludvig/mini-printf) and to [newlib](https://github.com/bminor/newlib).
## Downloads
UEFI bootable ISO files can be found on the [releases page](https://github.com/logern5/UEFI_Basic/releases).
## TODO:
The conversion from 8 to 16 bit chars, along with passing the *eST pointer with every outchar() causes overhead (change to global var)
Remove commented out lines
Remove now unneeded functions and files
