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
#ifndef _SYNCLOCK_
#define _SYNCLOCK_

#include <pthread.h>
#include <stdlib.h>

typedef struct synclock {
    pthread_mutex_t m;
    pthread_cond_t  c;
    unsigned char   s;
}synclock_t;

static synclock_t* synclock_create(void) 
{
    synclock_t *lock = (synclock_t*)malloc(sizeof(synclock_t));
    if( lock == NULL ) {
        return NULL;
    }        
    memset(lock, 0, sizeof(synclock_t));

    if (pthread_mutex_init(&lock->m,NULL) != 0) {
        free(lock);
        return NULL;
    }

    if (pthread_cond_init(&lock->c,NULL) != 0) {
        pthread_mutex_destroy(&lock->m);
        free(lock);
        return NULL;
    }
    lock->s = 0;

    return lock;
}

static void synclock_wait(synclock_t *lock) 
{
    int retval = 0;
    pthread_mutex_lock(&lock->m);
    while (!lock->s) {
        pthread_cond_wait(&lock->c, &lock->m);
    }
    lock->s = 0;
    pthread_mutex_unlock(&lock->m);
}

static void synclock_notify(synclock_t *lock) 
{    
    pthread_mutex_lock(&lock->m);
    lock->s = (unsigned char)1;
    pthread_cond_signal(&lock->c);
    pthread_mutex_unlock(&lock->m);
}

static void synclock_destroy(synclock_t *lock) 
{
    if(lock == NULL) {
        return;
    }        
    synclock_notify(lock);
    pthread_cond_destroy(&lock->c);
    pthread_mutex_destroy(&lock->m);
    free(lock);
}

#endif
