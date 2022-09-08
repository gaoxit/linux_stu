#ifndef __HAL_TASK__
#define __HAL_TASK__

#include <pthread.h>
#include "task_export.h"

#define MAX_TASK_NAME_LEN 16    // 最大的任务名长度

typedef enum
{
    TASK_NOT_CREATE = 0,
    TASK_CREATED,
    TASK_START,
}TASK_STATUS_E;

typedef struct
{
    task_id_t task_id;
    uint8_t task_status;
    uint8_t task_name[MAX_TASK_NAME_LEN];
    uint32_t task_priority;
    uint8_t *task_stack;
    uint32_t task_stack_size;
    task_callback_t task_callback;
    task_callback_para_t task_callback_para;
    task_init_t task_init;
} task_t;       //注意在}和自定义名称之间加空格，要不会报错

int32_t hal_task_create(task_t* task);
// int32_t hal_task_start(task_t* task);



#endif //__HAL_TASK__