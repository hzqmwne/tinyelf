# tinyelf

Generate super small but valid ELF executable file from C source code.  
It merges all sections (.text, .data, .rodata) in one, then copies it out, and finally adds ELF Header and Program Headers manually to generate a statically-linked file.  

I hope this project is helpful for writing shellcode by C, or for kernel pwn exploit writing.  

The project is firstly used at [here](https://github.com/hzqmwne/my-ctf-challenges/tree/master/0CTF_TCTF-2021-Finals/babalogin/src ).  

## Usage

Add xxx.c file here, include tinylib.h, modify Makefile, then `make`.  

It will generate three files for each .c file:  
- \*.elf: raw ELF created by gcc  
- \*.textbin: pure shellcode  
- \*.out: the tiny elf file  

(Need gcc 8 or later to support `-static-pie` option)


## nolibc

Refer to this LWN article: [Nolibc: a minimal C-library replacement shipped with the kernel](https://lwn.net/Articles/920158/ )  

It is a very interesting project, and it has appeared in Linux Kernel mainline since 5.1 !  
see [tools/include/nolibc](https://elixir.bootlin.com/linux/v6.2-rc4/source/tools/include/nolibc )  

This may be a better replacement of `tinylib.h` in this project.  
