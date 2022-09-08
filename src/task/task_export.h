#ifndef __TASK_EXPORT__
#define __TASK_EXPORT__

#include "platform.h"

typedef uint32_t task_id_t;
typedef void* task_callback_ret_t;
typedef void* task_callback_para_t;
typedef task_callback_ret_t (*task_callback_t)(task_callback_para_t);
typedef int32_t (*task_init_t)();

void task_init(void);






#endif  //__TASK_EXPORT__