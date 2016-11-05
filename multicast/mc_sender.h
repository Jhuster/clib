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
#ifndef _MC_SENDER_
#define _MC_SENDER_

#include <arpa/inet.h>

typedef struct _mc_sender {
    int m_socket;    
    struct sockaddr_in m_multicast_addr;
} mc_sender_t;

mc_sender_t *mc_sender_create(char *multicast_addr, int multicast_port);
void mc_sender_destroy(mc_sender_t * sender);
int mc_sender_send(mc_sender_t * sender, unsigned char *buffer, int len);

#endif //_MC_SENDER_
