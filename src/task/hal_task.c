#include "hal_task.h"

int32_t hal_task_create(task_t* task)
{
    pthread_t thread = 0;
    pthread_attr_t attribute;

    pthread_attr_init(&attribute);
    pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_DETACHED);

    // task->task_status = TASK_NOT_CREATE;

    int ret = -1;
    ret = pthread_attr_setstack(&attribute, task->task_stack, task->task_stack_size);
    if(ret != 0)
    {
        printf("pthread_attr_setstack(%s) error!\n", task->task_name);
        return false;
    }
    // printf("pthread_attr_setstack(%s) success!\n", task->task_name);

    ret = pthread_create(&thread, &attribute, task->task_callback, task->task_callback_para);
    // printf("pthread_create_thread = %lu!\n", thread);

    if(ret != 0)
    {
        printf("pthread_create(%s) error!\n", task->task_name);
        return false;
    }
    // printf("pthread_create(%s) success!\n", task->task_name);

    pthread_attr_destroy(&attribute);

    // ret = pthread_setname_np(thread, task->task_name);
    // if(ret != 0)
    // {
    //     printf("pthread_setname_np(%s) error!\n", task->task_name);
    //     return false;
    // }
    // printf("pthread_setname_np(%s) success!\n", task->task_name);

    if (task->task_priority > 0)
    {
        struct sched_param sched_para;
        // memset(&sched_para, 0x00, sizeof(struct sched_param));
        sched_para.sched_priority = task->task_priority;
        ret = pthread_setschedparam(thread, SCHED_FIFO, &sched_para);
        if (ret != 0)
        {
            //
        }
    }


    // task->task_id = thread;
    // task->task_status = TASK_CREATED;

    return true;
}

int32_t hal_task_start(task_t* task)
{
    task->task_status = TASK_START;

    return true;
}