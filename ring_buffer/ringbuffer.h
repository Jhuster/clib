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
 
#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include <pthread.h>

typedef struct _ring_buffer {
    unsigned char *buffer;
    int size;
    int in;
    int out;
    pthread_mutex_t lock;
} ring_buffer_t;

ring_buffer_t* ring_buffer_init(int capacity);
void ring_buffer_free(ring_buffer_t *rbuffer);
int ring_buffer_filled_bytes(ring_buffer_t *rbuffer);
int ring_buffer_unused_space(ring_buffer_t *rbuffer);
int ring_buffer_clear(ring_buffer_t *rbuffer);
int ring_buffer_get(ring_buffer_t *rbuffer, void *buffer, int size);
int ring_buffer_put(ring_buffer_t *rbuffer, void *buffer, int size);

#endif //_RING_BUFFER_H_
