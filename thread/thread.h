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
 
#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>
#include <stdlib.h>

typedef struct _thread {
    int m_is_started;
    int m_is_interrupted;
    pthread_t m_thread;
}thread_t;

typedef long THREAD;

static THREAD thread_create()
{
    thread_t * thread = (thread_t *)malloc(sizeof(thread_t));
    thread->m_is_interrupted = 0;
    thread->m_is_started = 0;
    return (THREAD)thread;
}

static void thread_destroy(THREAD handle)
{
    thread_t * thread = (thread_t *)handle;
    if(thread==NULL) {
        return;
    }
    free(thread);
}

static int thread_start(THREAD handle,void *(*looper)(void *),void *arg)
{
    thread_t * thread = (thread_t *)handle;
    if(thread==NULL) {
        return -1;
    }
    if(thread->m_is_started) {
        return 0;
    }
    thread->m_is_interrupted = 0;
    if(pthread_create(&thread->m_thread,NULL,looper,arg) != 0) {
        return -1;
    }
    thread->m_is_started = 1;
    return 0;
}

static int thread_is_started(THREAD handle)
{
    thread_t * thread = (thread_t *)handle;
    if(thread==NULL) {
        return 0;
    }
    return thread->m_is_started;
}

static int thread_is_interrupted(THREAD handle) 
{
    thread_t * thread = (thread_t *)handle;
    if(thread==NULL) {
        return 1;
    }
    return thread->m_is_interrupted;
}

static int thread_stop(THREAD handle,void (*interrupter)()) 
{
    thread_t * thread = (thread_t *)handle;
    if(thread==NULL) {
        return -1;
    }
    if(!thread->m_is_started) {
        return 0;
    }
    thread->m_is_interrupted = 1;
    if(interrupter!=NULL) {
        interrupter();
    }
    pthread_join(thread->m_thread,NULL);
    thread->m_is_started = 0;
    return 0;
}

static int thread_join(THREAD handle)
{
    thread_t * thread = (thread_t *)handle;
    if(thread==NULL || !thread->m_is_started) {
        return -1;
    }
    pthread_join(thread->m_thread,NULL);
    return 0;
}

#endif
