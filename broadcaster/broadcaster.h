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
#ifndef _BROADCASTER_H_
#define _BROADCASTER_H_

#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BROADCASTER_WORKMODE_SENDER 0
#define BROADCASTER_WORKMODE_RECVER 1

typedef struct _broadcaster {
    int m_workmode;
    int m_socket;    
    struct sockaddr_in m_addr;
}broadcaster_t;

broadcaster_t * broadcaster_create(int workmode, int port);
void broadcaster_destroy(broadcaster_t * broadcaster);
void broadcaster_interrupt(broadcaster_t * broadcaster);
int broadcaster_send_packet(broadcaster_t * broadcaster, unsigned char *buffer, int len);
int broadcaster_recv_packet(broadcaster_t * broadcaster, unsigned char *buffer, int len);

#endif