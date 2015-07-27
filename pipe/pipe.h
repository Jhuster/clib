/*
 *  COPYRIGHT NOTICE  
 *  Copyright (C) 2015, Jhuster, All Rights Reserved
 *  Author:  Jhuster(lujun.hust@gmail.com)
 *  
 *  https://github.com/Jhuster
 *   
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.  
 */
#ifndef _PIPE_H_
#define _PIPE_H_

#include <unistd.h>
#include <fcntl.h>

typedef struct _pipe {
    size_t size;
    int pipes[2];
}pipe_t;

typedef enum _pipe_mode {
    PIPE_BLOCKING,
    PIPE_NO_BLOCKING
}pipe_mode_t;

#define PIPE_SUCCESS 0
#define PIPE_FAILURE -1

static inline int pipe_open(pipe_t * piper, size_t size, pipe_mode_t mode) {
    
    if( pipe(piper->pipes) ) {
        return PIPE_FAILURE;
    }

    if( mode == PIPE_NO_BLOCKING ) {
        int flags[2];
        flags[0] = fcntl(piper->pipes[0], F_GETFL);
        flags[1] = fcntl(piper->pipes[1], F_GETFL);
        fcntl(piper->pipes[0], F_SETFL, flags[0] | O_NONBLOCK); //set pipe to no blocking mode
        fcntl(piper->pipes[1], F_SETFL, flags[1] | O_NONBLOCK); //set pipe to no blocking mode         
    }    

    piper->size = size;

    return PIPE_SUCCESS;
}

static inline int pipe_close(pipe_t * piper) {

    int ret = PIPE_SUCCESS;

    if( close(piper->pipes[0]) ) {
        ret = PIPE_FAILURE;
    }

    if( close(piper->pipes[1]) ) {
        ret = PIPE_FAILURE;
    }

    return ret;
}

static inline int pipe_get(pipe_t * piper, void *buffer) {

    if( read(piper->pipes[0], buffer, piper->size) != piper->size ) {
        return PIPE_FAILURE;
    }

    return PIPE_SUCCESS;
}

static inline int pipe_put(pipe_t * piper, void *buffer) {

    if( write(piper->pipes[1], buffer, piper->size) != piper->size ) {
        return PIPE_FAILURE;
    }

    return PIPE_SUCCESS;
}

#endif
