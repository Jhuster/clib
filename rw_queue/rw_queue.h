/*
 *  COPYRIGHT NOTICE  
 *  Copyright (C) 2015, Jhuster, All Rights Reserved
 *  Author:  Jhuster(lujun.hust@gmail.com)
 *  
 *  https://github.com/Jhuster/clib
 *   
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.  
 */
#ifndef _RW_QUEUE_
#define _RW_QUEUE_

#include <pthread.h>
#include <stdlib.h>

typedef struct _rw_queue {
    int rp;
    int wp;
    int count;
    int capacity; 
    int interrupt;
    pthread_mutex_t mutex;
    pthread_cond_t  rcond;
    pthread_cond_t  wcond;
} rw_queue_t;

static rw_queue_t* rw_queue_open(int capacity)
{
    rw_queue_t *queue = (rw_queue_t*)malloc(sizeof(rw_queue_t));
    if (queue == NULL) {
        return NULL;
    }        
    memset(queue, 0, sizeof(rw_queue_t));

    if (pthread_mutex_init(&queue->mutex,NULL) != 0) {
        free(queue);
        return NULL;
    }

    if (pthread_cond_init(&queue->rcond,NULL) != 0) {        
        free(queue);
        return NULL;
    }

    if (pthread_cond_init(&queue->wcond,NULL) != 0) {        
        free(queue);
        return NULL;
    }

    queue->rp = 0;
    queue->wp = 0;
    queue->count = 0;
    queue->interrupt = 0;
    queue->capacity = capacity;

    return queue;
}

static int rw_queue_is_empty(rw_queue_t *queue)
{
    return queue->count == 0;
}

static int rw_queue_is_full(rw_queue_t *queue)
{
    return queue->count == queue->capacity;
}

static int rw_queue_count(rw_queue_t *queue)
{
    return queue->count;
}

static int rw_queue_get_wp(rw_queue_t *queue,int isblock)
{
    int idx = -1;
    pthread_mutex_lock(&queue->mutex);
    while (!queue->interrupt && rw_queue_is_full(queue)) {
        if (!isblock) {
            pthread_mutex_unlock(&queue->mutex);
            return -1;
        }   
        pthread_cond_wait(&queue->wcond, &queue->mutex); 
    }        
    idx = queue->wp;
    pthread_mutex_unlock(&queue->mutex);
    return queue->interrupt?-1:idx;
}

static void rw_queue_put_wp(rw_queue_t *queue)
{
    pthread_mutex_lock(&queue->mutex);
    queue->wp = (queue->wp +1) % queue->capacity;
    queue->count++;
    pthread_cond_signal(&queue->rcond);
    pthread_mutex_unlock(&queue->mutex);
}

static int rw_queue_get_rp(rw_queue_t *queue,int isblock)
{
    int idx = 0;
    pthread_mutex_lock(&queue->mutex);
    while (!queue->interrupt && rw_queue_is_empty(queue)) {
        if (!isblock) {
            pthread_mutex_unlock(&queue->mutex);
            return -1;
        } 
        pthread_cond_wait(&queue->rcond, &queue->mutex);
    }
    idx = queue->rp;
    pthread_mutex_unlock(&queue->mutex);
    return queue->interrupt?-1:idx;
}

static void rw_queue_put_rp(rw_queue_t *queue)
{
    pthread_mutex_lock(&queue->mutex);
    queue->rp = (queue->rp +1) % queue->capacity;
    queue->count--;
    pthread_cond_signal(&queue->wcond);
    pthread_mutex_unlock(&queue->mutex);
}

static void rw_queue_interrupt(rw_queue_t *queue)
{
    if (queue->interrupt) {
        return;
    }
    pthread_mutex_lock(&queue->mutex);
    queue->interrupt = 1;
    pthread_cond_signal(&queue->rcond);
    pthread_cond_signal(&queue->wcond);
    pthread_mutex_unlock(&queue->mutex);
}

static void rw_queue_close(rw_queue_t *queue)
{
    rw_queue_interrupt(queue);

    pthread_cond_destroy(&queue->rcond);
    pthread_cond_destroy(&queue->wcond);
    pthread_mutex_destroy(&queue->mutex);

    free(queue);
}

#endif
