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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "ringbuffer.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))

ring_buffer_t* ring_buffer_init(int capacity)
{
    if (!is_power_of_2(capacity)) {
        return NULL;
    }

    ring_buffer_t *rbuffer = (ring_buffer_t *) malloc(sizeof(ring_buffer_t));
    if (!rbuffer) {
        return rbuffer;
    }

    rbuffer->buffer = (unsigned char *) malloc(capacity*sizeof(char));
    rbuffer->size = capacity;
    rbuffer->in  = 0;
    rbuffer->out = 0;
    pthread_mutex_init(&rbuffer->lock, NULL);

    return rbuffer;
}

void ring_buffer_free(ring_buffer_t *rbuffer)
{
    if (rbuffer == NULL) {
        return;
    }

    if (rbuffer->buffer) {
        free(rbuffer->buffer);
        rbuffer->buffer = NULL;
    }

    free(rbuffer);
}

int ring_buffer_clear(ring_buffer_t *rbuffer)
{
    pthread_mutex_lock(&rbuffer->lock);
    rbuffer->in = rbuffer->out = 0;
    pthread_mutex_unlock(&rbuffer->lock);
    return 0;
}

int ring_buffer_filled_bytes(ring_buffer_t *rbuffer)
{
    int len = 0;
    pthread_mutex_lock(&rbuffer->lock);
    len = rbuffer->in - rbuffer->out;
    pthread_mutex_unlock(&rbuffer->lock);
    return len;
}

int ring_buffer_unused_space(ring_buffer_t *rbuffer)
{
    int used = ring_buffer_filled_bytes(rbuffer);
    return rbuffer->size - used;
}

int ring_buffer_get(ring_buffer_t *rbuffer, void *buffer, int size)
{
    assert(rbuffer || buffer);

    pthread_mutex_lock(&rbuffer->lock);

    size = min(size, rbuffer->in - rbuffer->out);
    int len = min(size, rbuffer->size - (rbuffer->out & (rbuffer->size - 1)));

    memcpy(buffer, rbuffer->buffer + (rbuffer->out & (rbuffer->size - 1)), len);
    memcpy(buffer + len, rbuffer->buffer, size - len);
    rbuffer->out += size;

    if (rbuffer->in == rbuffer->out)
        rbuffer->in = rbuffer->out = 0;

    pthread_mutex_unlock(&rbuffer->lock);

    return size;
}

int ring_buffer_put(ring_buffer_t *rbuffer, void *buffer, int size)
{
    assert(rbuffer || buffer);

    pthread_mutex_lock(&rbuffer->lock);

    size = min(size, rbuffer->size - rbuffer->in + rbuffer->out);
    int len  = min(size, rbuffer->size - (rbuffer->in & (rbuffer->size - 1)));

    memcpy(rbuffer->buffer + (rbuffer->in & (rbuffer->size - 1)), buffer, len);
    memcpy(rbuffer->buffer, buffer + len, size - len);
    rbuffer->in += size;

    pthread_mutex_unlock(&rbuffer->lock);

    return size;
}
