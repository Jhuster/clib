#include <stdio.h>
#include "thread.h"

#define LOG(format, ...) printf(format, ##__VA_ARGS__)

typedef struct _echo {
    THREAD thread;
    int num;
} echo_t;

void *echo_loop(void *arg) {

    echo_t *echo = (echo_t *) arg;
    
    LOG("\nEnter echo loop !\n\n");

    while (!thread_is_interrupted(echo->thread)) {
        LOG("echo %d\n", echo->num++);
        sleep(1);
    }

    LOG("\nLeave echo loop !\n\n");    
}

int main(int argc, char const *argv[])
{
    echo_t echo;
    echo.num = 0;

    echo.thread = thread_create();
    thread_start(echo.thread, echo_loop,&echo);

    getchar();
    getchar();

    thread_stop(echo.thread, NULL);
    thread_destroy(echo.thread);

    return 0;
}