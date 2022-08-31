#define LIB_STU         0   //装载、链接与库学习代码
#define MY_PRINTF       0   //移植的printf函数功能验证调试
#define SNPRINTF        0   //snprintf函数功能验证
#define DIR_FILE        1   //删除文件夹功能验证


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



#include <stdio.h>
#include "myprintf.h"
#include "file_infc.h"

int main(void)  
{
    #if MY_PRINTF
    myprintf("%d, %x, %f, %c, %s, %%, %a\n", 3210, 2748, 3.14, 'B', "hello");
    #endif


    #if SNPRINTF
    char buffer[10];
    char* s = "adcdefg";
    int len = snprintf(buffer, 3, "%s", s);

    printf("len = %d\nbuffer = %s\n", len, buffer);
    /*运行结果：
        1.返回值都为s的实际大小，不加最后的\0，本例中为7;
        2.当传入的size小于s的大小时，实际复制到buffer中的字符个数为size-1
        3.当传入的size大于s的大小时，实际复制到buffer中的字符为全部s中的字符
    */
    #endif

    #if DIR_FILE
        del_dir("/mnt/e/1Code/my_code/linux_stu/tmp_for_del");
        create_dir("/mnt/e/1Code/my_code/linux_stu/tmp_for_del/tmp");
    #endif



    return 0;  
}
