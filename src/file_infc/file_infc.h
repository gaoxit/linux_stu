#ifndef _FILE_INFC_
#define _FILE_INFC_
#include <stdbool.h>

/**
 * @brief 删除当前文件下的所有文件和非空文件夹
 * @note
 * @param path 目录名
 * @return true 删除成功
 * @return false 删除失败
 */
bool del_dir(char* path);




#endif //_FILE_INFC_