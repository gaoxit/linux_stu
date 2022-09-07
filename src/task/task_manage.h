#ifndef __TASK_MANAGE__
#define __TASK_MANAGE__

#include <stdio.h>
typedef enum
{
    APP_TASK_CYCLE1 = 0,
    APP_TASK_CYCLE2,
    APP_TASK_ID_MAX
} APP_TASK_ID_E;

void task_create(void);
// void task_start(void);



#endif  //__TASK_MANAGE__