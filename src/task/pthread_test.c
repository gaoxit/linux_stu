#include <pthread.h>
#include <stdio.h>

#include <string.h>

#include <unistd.h>
void * Thread_Callback()
{
    static int cnt =1;
    printf("Create Thread: %d\n", cnt);
    cnt++;
}

int main(void)
{
    int err = 0;
    pthread_t tid;
    pthread_attr_t attr;
    size_t stack_size = 0;
    while(1)
    {
        pthread_attr_init(&attr);
        pthread_attr_setstacksize(&attr, 102400);

        err = pthread_create(&tid, &attr, Thread_Callback, NULL);

        pthread_attr_getstacksize(&attr, &stack_size);

        pthread_attr_destroy(&attr);
        if(err != 0)
        {
            printf("Can't create thread: %s\n", strerror(err));
            break;
        }

        printf("%d: stack_size: %dk\n", (int)tid, (int)stack_size);

	    usleep(2000);
    }
}

