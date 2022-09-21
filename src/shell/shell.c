#include "shell.h"
#define MAX_STR_LEN 256

int shell_printf_input(void)
{
    char str[MAX_STR_LEN];
    
    while(1)
    {
        printf("Please input: ");
        plm_tlog_error("SHELL_LOG", "Please input: ");
        memset(str, 0, sizeof(str));
        
        // if(scanf("%c", str) != 1)    //如果输入是%c形式的，则只能输入一个字符，最后还会对结束符\0打印
        if(scanf("%s", str) != 1)
        {
            printf("Please input string!");
        }
        printf("Your input is: %s\n", str);
        plm_tlog_error("SHELL_LOG", "Your input is: %s", str);
    }
    return RES_OK;
}