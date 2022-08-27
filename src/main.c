

#define LIB_STU 0
#define MAIN 1

#if LIB_STU
//装载、链接与库学习代码
int printf(const char* format, ...);
int g_init_var = 84; //0x54
int g_uninit_var;
void func1(int i)
{
    printf("%d\n", i);
}
int main(void)
{
    static int static_var = 85;
    static int static_var2;

    int a = 1;
    // int b;
    int b = 2;
    func1(static_var + static_var2 + a + b);
    return a;
}

/*
gxt@LAPTOP-JM9VJP8V:/mnt/e/1Code/my_code/linux_stu/src$ objdump -h main.o

main.o:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000055  0000000000000000  0000000000000000  00000040  2**0          40+55=95 下一段的起始地址却为98，不连续？
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000008  0000000000000000  0000000000000000  00000098  2**2          
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000004  0000000000000000  0000000000000000  000000a0  2**2
                  ALLOC
  3 .rodata       00000004  0000000000000000  0000000000000000  000000a0  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .comment      00000036  0000000000000000  0000000000000000  000000a4  2**0
                  CONTENTS, READONLY
  5 .note.GNU-stack 00000000  0000000000000000  0000000000000000  000000da  2**0
                  CONTENTS, READONLY
  6 .eh_frame     00000058  0000000000000000  0000000000000000  000000e0  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA



gxt@LAPTOP-JM9VJP8V:/mnt/e/1Code/my_code/linux_stu/src$ size main.o
   text    data     bss     dec     hex filename
    177       8       4     189      bd main.o                          text段55对应的十进制是85，这里是177


*/
#endif

#include "myprintf.h"
int main(void)  
{  
    myprintf("%d, %x, %f, %c, %s, %%, %a\n", 3210, 2748, 3.14, 'B', "hello");  
    return 0;  
}
