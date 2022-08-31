#ifndef _FILE_INFC_
#define _FILE_INFC_
#include <stdbool.h>
#include <sys/stat.h>   //mkdir函数所在的头文件
#include <unistd.h>     //F_OK等宏所在的头文件

/**
 * @brief 创建文件夹
 * @note
 * @param path 目录名
 * @return true 创建成功
 * @return false 创建失败
 */
bool create_dir(char *path);

/**
 * @brief 删除当前文件下的所有文件和非空文件夹
 * @note
 * @param path 目录名
 * @return true 删除成功
 * @return false 删除失败
 */
bool del_dir(char* path);




#endif //_FILE_INFC_