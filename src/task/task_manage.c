#include "task_manage.h"
#include "hal_task.h"
#include "hal_timer.h"
#include "utils_export.h"


#define STACK_SIZE_1K 1024
#define STACK_SIZE_1M (STACK_SIZE_1K * 1024)
#define STACK_SIZE_2M (STACK_SIZE_1M * 2)


static uint8_t stack_task_cycle1[STACK_SIZE_2M];
static uint8_t stack_task_cycle2[STACK_SIZE_2M];


task_callback_ret_t task_cycle1(task_callback_para_t arg);
task_callback_ret_t task_cycle2(task_callback_para_t arg);


static task_t g_task_list[] = 
{
    {
        .task_id = APP_TASK_CYCLE1,
        .task_status = TASK_NOT_CREATE,
        .task_name = "cycle1",
        .task_stack = stack_task_cycle1,
        .task_priority = 1,
        .task_stack_size = sizeof(stack_task_cycle1),
        .task_callback = task_cycle1,
        .task_callback_para = 0,
        .task_init = NULL,
    },
    {
        .task_id = APP_TASK_CYCLE2,
        .task_status = TASK_NOT_CREATE,
        .task_name = "cycle2",
        .task_stack = stack_task_cycle2,
        .task_priority = 1,
        .task_stack_size = sizeof(stack_task_cycle2),
        .task_callback = task_cycle2,
        .task_callback_para = 0,
        .task_init = NULL,
    },
};



task_callback_ret_t task_cycle1(task_callback_para_t arg)
{
    standard_time_t now_time;
    
    while(1)
    {
        hal_get_time_in_standard(&now_time);
        printf("当前在task1任务中--当前时间是：20%d年%d月%d日 %d:%d:%d,%d\n",
        now_time.year,
        now_time.month,
        now_time.mday,
        now_time.hour,
        now_time.min,
        now_time.s,
        now_time.ms);

        hal_sleep_in_s(1);
    }
    return NULL;
}


task_callback_ret_t task_cycle2(task_callback_para_t arg)
{
    int num = 0;
    while(1)
    {
        printf("当前在task2任务中....(%d)..\n", num++);
        hal_sleep_in_s(1);
    }
    return NULL;
}


void task_create(void)
{
    uint8_t ret = 0;
    for(uint8_t task_index = 0; task_index < COUNT(g_task_list); task_index++)
    {
        ret = hal_task_create(&g_task_list[task_index]);
        if(ret != 0)
        {
            printf("Create %s failure......\n", g_task_list[task_index].task_name);
        }
    }
}

// void task_start(void)
// {
//     uint8_t ret = 0;
//     for(uint8_t task_index = 0; task_index < COUNT(g_task_list); task_index++)
//     {
//         ret = hal_task_start(&g_task_list[task_index]);
//         if(ret != 0)
//         {
//             //
//         }
//     }
// }