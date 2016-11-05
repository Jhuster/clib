#include <stdio.h>
#include <string.h>
#include "synclock.h"

static volatile int g_system_exit = 0;

typedef struct _queue {    
    int number;
} queue_t;

typedef struct _syncQueue {
    queue_t queue;
    synclock_t *lock;
} sync_queue_t;

static void * reading_thread(void * arg) 
{
    sync_queue_t *squeue = (sync_queue_t *)arg;

    while (!g_system_exit) {       
        if (squeue->queue.number == 0) {
            synclock_wait(squeue->lock);            
        }
        printf("number: %d \n",squeue->queue.number);
        squeue->queue.number--;
    }

    return (void *)0;
}

static void * writing_thread(void * arg) 
{
    sync_queue_t *squeue = (sync_queue_t *)arg;

    while (!g_system_exit) {    
        sleep(1);   
        squeue->queue.number++;            
        synclock_notify(squeue->lock);
    }

    return (void *)0;
}

int main(int argc, char *argv[]) {

    sync_queue_t squeue;
    squeue.lock = synclock_create();    
    squeue.queue.number = 0;

    pthread_t rthread,wthread;
    if (pthread_create(&rthread,NULL,reading_thread,&squeue) != 0) {
        printf("pthread create reading thread failed ! \n");
        return -1;
    }
    if (pthread_create(&wthread,NULL,writing_thread,&squeue) != 0) {
        printf("pthread create writing thread failed ! \n");
        return -1;
    }

    char command;
    do {
        command = getchar();
    } while (command!='q');

    g_system_exit = 1;

    pthread_join(rthread,NULL);
    pthread_join(wthread,NULL);

    synclock_destroy(squeue.lock);    

    printf("byte byte !\n\n");            

    return 0;
}
