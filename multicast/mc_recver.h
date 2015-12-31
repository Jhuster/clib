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
#ifndef _MC_RECVER_
#define _MC_RECVER_

#include <arpa/inet.h>

typedef struct _mc_recver {
    int m_socket;    
    struct sockaddr_in m_localaddr;
} mc_recver_t;

mc_recver_t *mc_recver_create(char *multicast_addr,int multicast_port);
void mc_recver_destroy(mc_recver_t * recver);
int mc_recver_recv(mc_recver_t * recver, unsigned char *buffer, int len);

#endif //_MC_RECVER_