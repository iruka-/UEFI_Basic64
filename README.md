# UEFI Basic
A simple BASIC language intepreter that runs on UEFI. Feel free to submit a pull request.
## Requirements
The gcc compiler for x86\_64 mingw platform (x86\_64-w64-mingw32-gcc, this can be found in the Debian package mingw-w64).
## Compilation
Running `make` will produce an EFI file and a bootable ISO for the x86\_64 architechture.
## Usage
Programs need to be typed in with line numbers. To run use the `run` command, and to create a new program use the `new` command. `list` prints out the current program.
## Credits
Credit to [gnu-efi](https://github.com/vathpela/gnu-efi) and TinyBsaicPlus.
## Downloads
UEFI bootable ISO files can be found on the [releases page](https://github.com/logern5/UEFI_Basic/releases).
## Language info
The version of BASIC used in this project is a C version of TinyBASIC, a highly portable BASIC interpreter which only depends on a couple external functions for IO.
A list of all the commands and functions can be located [here](src/ubasic/README.md). This version of BASIC included runs on desktop operating systems, UEFI, and
Arduino.

All variables are a single letter and are `short`s (signed integers, and 16-bit on most implementations). Memory ddresses for the PEEK and POKE commands are also `short`s.
The addresses are indexes in a C array. Negative addresses can be used as they will simply be converted to their 2's complement, accessing higher memory than the
positive addresses. POKE takes an argument of a 8-bit integer and PEEK returns an 8-bit integer. PEEK and POKE can both be used to access memory for purposes such as 
strings, arrays, etc, which are not natively supported in the BASIC implementation.
## TODO:
Running the efi file sometimes gives a load error. I can run it and rerun it, but after resetting the VM, I get "Command Error Status: Load error". 
The md5sum seems to be different before starting the VM and after closing it. Creating a backup file seems to fix it. The error occurs only after a hard reset
or shutdown.

Remove commented out lines

Add printing and reading char function, so they can store to memory.
