#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "utils.h"
#include "file_infc.h"

bool check_file_exist(char* path)
{
    return true;
}

bool del_dir(char* path)
{
    if (check_file_exist(path) == false)
    {
        return false;
    }

    char cur_path[256] = {0};
    snprintf(cur_path, sizeof(cur_path), "%s", path);

    //手动在末尾添加\0
    if ((strlen(path) + 1 < sizeof(cur_path)) && (cur_path[strlen(path) - 1] == '/'))
    {
        cur_path[strlen(path) + 1] = '\0';
    }
    printf("cur_path1 = %s\n", cur_path);

    DIR* dir = opendir(path);
    struct dirent* dp = NULL;

    // if ((dp = readdir(dir)) == NULL)
    // {
    //     printf("cur_path is null!\n");
    // }
    // else if((dp = readdir(dir)) != NULL)
    // {
    //     printf("cur_path is not null!\n");
    // }

    while ((dp = readdir(dir)) != NULL) //readdir(dir)返回参数dir 目录流的下个目录进入点
    {
        printf("dp->d_name:%s\n", dp->d_name);
        //1.如果是.或者..则跳过本次循环
        if(!strcmp(".", dp->d_name) || !strcmp("..", dp->d_name))
        {
            continue;
        }
        //2.遍历当前文件夹下所有的文件，进行删除，不包括非空文件夹
        strncat(cur_path, "/", (sizeof(cur_path) - 1 - strlen(cur_path)));
        // printf("cur_path2 = %s\n", cur_path);

        strncat(cur_path, dp->d_name, (sizeof(cur_path) - 1 - strlen(cur_path)));
        printf("cur_path3 = %s\n", cur_path);

        CHECK(remove, cur_path);
        //3.重新将当前文件目录赋值给cur_path
        snprintf(cur_path, sizeof(cur_path), "%s", path);
        printf("cur_path4 = %s\n", cur_path);
    }
    closedir(dir);
    return true;

    //此代码只能删除一级目录下的文件和空文件夹，当一级目录下的文件夹中有文件时，则子文件夹和其中的文件都不会删除
}