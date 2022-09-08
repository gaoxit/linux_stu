#ifndef __LOG_EXPORT__
#define __LOG_EXPORT__

#include <time.h>
#include <pthread.h>
#include "platform.h"
#include "log_utils.h"
#include "task_export.h"


typedef char TEXT;
typedef pthread_mutex_t mutex_lock_t;
typedef pthread_cond_t mutex_cond_t;

/// @brief 获取对应文件的大小
/// @param path 
/// @return 对应文件的大小；若文件不存在，返回-1
int lcp_get_file_size(const char *path);

task_callback_ret_t tlog_thread(task_callback_para_t arg);




#endif //__LOG_EXPORT__