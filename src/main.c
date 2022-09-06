#define LIB_STU         0   //装载、链接与库学习代码
#define MY_PRINTF       0   //移植的printf函数功能验证调试
#define SNPRINTF        0   //snprintf函数功能验证
#define DIR_FILE        0   //删除文件夹功能验证
#define TIMER           0   //timer相关功能单元测试
#define TASK            1   //任务相关功能

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
#include <stdint.h>
#include <pthread.h>
#include "myprintf.h"
#include "file_infc.h"
#include "hal_timer.h"
#include "task_manage.h"

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
        create_dir("/mnt/e/1Code/my_code/linux_stu/tmp_for_del/new_dir_create_by_code");
    #endif

    #if TIMER

    uint64_t now_ms_tick = hal_get_system_tick_in_ms();
    printf("now_ms_tick = %lu\n",now_ms_tick);
    uint64_t now_ms_time = hal_get_time_in_ms();
    printf("now_ms_time = %lu\n",now_ms_time);

    printf("now_ms_time - now_ms_tick = %lu\n", (now_ms_time- now_ms_tick));  //绝对时间减相对时间的差值基本不变

    standard_time_t now_time;
    hal_get_time_in_standard(&now_time);
    printf("当前时间是：20%d年%d月%d日 %d:%d:%d,%d\n",
            now_time.year,
            now_time.month,
            now_time.mday,
            now_time.hour,
            now_time.min,
            now_time.s,
            now_time.ms);

    hal_sleep_in_s(5);
    hal_get_time_in_standard(&now_time);
    printf("当前时间2是：20%d年%d月%d日 %d:%d:%d,%d\n",
            now_time.year,
            now_time.month,
            now_time.mday,
            now_time.hour,
            now_time.min,
            now_time.s,
            now_time.ms);


    #endif

    #if TASK
    task_init();    //ps -ef | grep my_linux.bin
    while(1)    //主进程不能退出，所以需要加while循环
    {
        static int i = 0;
        i++;
        printf("\033[31m main主线程中：i = %d \033[0m\n", i);

        hal_sleep_in_s(1);
    }
    #endif

    return 0;  
}
