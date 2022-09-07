#include "hal_task.h"

int32_t hal_task_create(task_t* task)
{
    pthread_t thread = 0;
    int ret = -1;
    pthread_attr_t attribute;

    pthread_attr_init(&attribute);
    pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_DETACHED);   
    /*
    设置线程的属性(joinable:可结合的；detached:分离的):
        1.可结合的线程能够被其他线程收回其资源和杀死，在被其他线程回收之前，它的存储器资源时不释放的；
        2.分离的线程是不能被其他线程回收或杀死的，它的存储器资源在它终止时由系统自动释放；
    */

    //设置线程栈
    ret = pthread_attr_setstack(&attribute, task->task_stack, task->task_stack_size);
    if(ret != 0)
    {
        printf("pthread_attr_setstack(%s) error!\n", task->task_name);
        return RES_ERR;
    }

    //创建线程
    ret = pthread_create(&thread, &attribute, task->task_callback, task->task_callback_para);
    if(ret != 0)
    {
        printf("pthread_create(%s) error!\n", task->task_name);
        return RES_ERR;
    }

    //删除线程属性  todo不知道为什么放在这里，不应该放到最后吗
    pthread_attr_destroy(&attribute);

    //设置线程名称
    // ret = pthread_setname_np(thread, task->task_name);
    // if(ret != 0)
    // {
    //     printf("pthread_setname_np(%s) error!\n", task->task_name);
    //     return RES_ERR;
    // }
    // printf("pthread_setname_np(%s) success!\n", task->task_name);

    //写入优先级
    if (task->task_priority > 0)
    {
        struct sched_param sched_para;
        memset(&sched_para, 0x00, sizeof(struct sched_param));
        sched_para.sched_priority = task->task_priority;
        ret = pthread_setschedparam(thread, SCHED_FIFO, &sched_para);
        if (ret != 0)
        {
            printf("\033[31mpthread_setschedparam(%s) error!\033[0m\n", task->task_name);
            return RES_ERR;
        }
    }

    return RES_OK;
}


// int32_t hal_task_start(task_t* task)
// {
//     task->task_status = TASK_START;

//     return RES_OK;
// }