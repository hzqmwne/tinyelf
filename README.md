# tinyelf

Generate super small but valid ELF executable file from C source code.  
It merges all sections (.text, .data, .rodata) in one, then copies it out, and finally adds ELF Header and Program Headers manually to generate a statically-linked file.  

I hope this project is helpful for writing shellcode by C, or for kernel pwn exploit writing.  

The project is firstly used at [here](https://github.com/hzqmwne/my-ctf-challenges/tree/master/0CTF_TCTF-2021-Finals/babalogin/src ).  

## Usage

Add xxx.c file here, include tinylib.h, modify Makefile, then `make`.  

It will generate three files for each .c file:
\*.elf: raw ELF created by gcc
\*.textbin: pure shellcode
\*.out: the tiny elf file

