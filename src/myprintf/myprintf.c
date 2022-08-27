#define SNPRINTF_STU 1
#define VSNPRINTF 0

#if SNPRINTF_STU
#include <stdio.h>
#include <stdarg.h>

void myitoa(int n, char str[], int radix); //将整数n表示成字符形式
void myprintf(const char *format, ...); 



void myitoa(int n, char str[], int radix) //将整数n表示成字符形式
{
    int remain;
    int num = 0, j = 0; 
    char tmp; //用于整理生成的字符格式
    // while (n /= radix)  //必须用do while形式，否则第一次n /= radix为0时则不进行字符串的转化
    do
    {
        remain = n % radix; 
        if (remain > 9)
        {
            str[num] = remain - 10 + 'A';
        }
        else
        {
            str[num] = remain + '0';
        }
        num++;
    }while(n /= radix);
    
    str[num] = '\0';                        //添加最后的结束符
    for (num--, j = 0; j < num; num--, j++) //因为最后num执行了++操作，所以先进行num--
    {
        tmp = str[j];
        str[j] = str[num];
        str[num] = tmp;
    }
}

void myprintf(const char *format, ...)  
{  
    char c, ch, str[30];
    //type char* va_list;
    va_list ap;  

    // #define _INITSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
    // #define va_start(ap, v) (ap = (va_list)&v + _INITSIZEOF(V))  //得到可变参数第一个参数的首地址
    va_start(ap, format);                       //使va_list类型变量ap指向可变参数表中的第一个参数format
    while((c = *format))                        //当走到format最后一个字符时结束循环
    {  
        switch(c)  
        {  
        case '%':                               //如果字符为%，则继续解析后面的类型
            ch = *++format;
            switch(ch)
            {  
            case 'd':                           //%d，说明是整型数据，按整型的规则解析
                {
                    // #define va_arg(ap, type) (*(type*) ((ap += _INITSIZEOF(type)) - _INITSIZEOF(type)))
                    //va_ag宏的作用：1、返回ap所指向对象的值；2、修改参数指针ap使其增加以指向表中的下一个参数(第二个参数提供了修改参数指针锁必须的信息)
                    int n = va_arg(ap, int);    //表明可变参数中有整数，输出
                    myitoa(n, str, 10);
                    fputs(str, stdout);
                    break;
                }  
            case 'x':                               //%x 十六进制输出整数
                {  
                    int n = va_arg(ap, int);
                    myitoa(n, str, 16);
                    fputs(str, stdout);
                    break;
                }  
            case 'f':
                {  
                    double f = va_arg(ap, double);  //%f 输出浮点数
                    int n;
                    n = f;
                    myitoa(n, str, 10);
                    fputs(str, stdout);
                    putchar('.');
                    n = (f - n) * 1000000;
                    myitoa(n, str, 10);
                    fputs(str, stdout);
                    break;  
                }  
            case 'c':                               //%c 输出字符型
                {  
                    putchar(va_arg(ap, int));
                    break;  
                }  
            case 's':                               //%s 输出字符串类型
                {  
                    char *p = va_arg(ap, char *);
                    fputs(p, stdout);
                    break;
                }  
            case '%':	                            //%%  输出%
                {  
                    putchar('%');
                    break;  
                }  
            default:  
                {  
                    fputs("format invalid!", stdout);  
                    break;  
                }  
            }  
            break;  
        default:  
            putchar(c);  
            break;  
        }  
        format++;  
    }
    //#define va_end(ap) (ap = (va_list)0)
    va_end(ap);         //va_end必须在va_arg读完所有参数后调用，用于被调函数完成正常返回，将指针ap赋值为0
}

#endif


#if VSNPRINTF
#include <stdio.h>
#include <stdarg.h>

void vsnprintf_call(const char* format, ...)
{ 
    char buffer[255] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 255, format, args);
    perror(buffer);         //perror显示错误的原因

    va_end(args);
}

int main(void)
{
    int n = 5;
    char name[] = "duoduo";

    vsnprintf_call("n = %d\n", n);
    vsnprintf_call("my name is %s\n", name);

    char filename[] = "./Make";
    FILE* pFile;
    pFile = fopen(filename,"r");
    if(pFile == NULL)
    {
        vsnprintf_call("Open %s failure\n", filename);    //若filename不存在，则会打印Open ./Make failure: No such file or directory
    }
    else
    {
        fclose(pFile);
    }

    return 0;
}









#endif