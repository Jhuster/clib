#include <stdio.h>
#include <string.h>
#include "rw_queue.h"

#define MAX_BUFFER_NUM 3
#define LOG(format, ...) printf(format, ##__VA_ARGS__)

static volatile int g_system_exit = 0;

static char buffer[MAX_BUFFER_NUM][1024];

static void * reading_thread(void * arg) 
{
    rw_queue_t *queue = (rw_queue_t *)arg;

    while (!g_system_exit) {       
        int idx = rw_queue_get_rp(queue, 1);
        LOG("Reader: %d avalable now, read it...\n", idx);
        /*
         * do something such as memcpy(dstbuf, buffer[idx], len);
         */
        rw_queue_put_rp(queue);    
        LOG("Reader: process ok, put %d back to queue \n", idx);
        //sleep(3);
    }

    return (void *)0;
}

static void * writing_thread(void * arg) 
{
    rw_queue_t *queue = (rw_queue_t *)arg;

    while (!g_system_exit) {    
        int idx = rw_queue_get_wp(queue, 1);
        LOG("Writer: %d idle now, write it ...\n", idx);
        /*
         * do something such as memcpy(buffer[idx], srcbuf, len);
         */
        rw_queue_put_wp(queue);    
        LOG("Writer: %d is avalable in queue \n", idx);
        sleep(2);
    }

    return (void *)0;
}

int main(int argc, char *argv[]) 
{
    rw_queue_t * queue = rw_queue_open(3);

    pthread_t rthread, wthread;
    if (pthread_create(&rthread, NULL, reading_thread, queue) != 0) {
        LOG("pthread create reading thread failed ! \n");
        return -1;
    }
    if (pthread_create(&wthread, NULL, writing_thread, queue) != 0) {
        LOG("pthread create writing thread failed ! \n");
        return -1;
    }

    char command;
    do {
        command = getchar();
    } while (command!='q');

    g_system_exit = 1;

    rw_queue_interrupt(queue);

    pthread_join(rthread, NULL);
    pthread_join(wthread, NULL);

    rw_queue_close(queue);    

    LOG("byte byte !\n\n");            

    return 0;
}
