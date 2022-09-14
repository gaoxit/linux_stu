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

#define TLOG_LEVEL_DEBUG            0
#define TLOG_LEVEL_INFO             1
#define TLOG_LEVEL_WARN             2
#define TLOG_LEVEL_ERROR            3
#define TLOG_LEVEL_FATAL            4
#define TLOG_LEVEL_OFF              5





/// @brief 获取对应文件的大小
/// @param path 
/// @return 对应文件的大小；若文件不存在，返回-1
int lcp_get_file_size(const char *path);

task_callback_ret_t tlog_thread(task_callback_para_t arg);

int32_t plm_tlog(const char* tag, uint32_t level, const char* func, uint64_t line, const char* format, ...);

#define plm_tlog_error(tag, ...) \
    plm_tlog(tag, TLOG_LEVEL_ERROR, __func__, __LINE__, __VA_ARGS__)

void tlog_init(void);



#endif //__LOG_EXPORT__