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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mc_recver.h"

#define LOG(format,...) printf(format,##__VA_ARGS__)

mc_recver_t *mc_recver_create(char *multicast_addr,int multicast_port)
{
    mc_recver_t *recver = (mc_recver_t *)malloc(sizeof(mc_recver_t));

    /* create socket handler*/    
    if((recver->m_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {     
        LOG("failed to create socket !\n");   
        free(recver);
        return NULL;
    }

    /* Enable SO_REUSEADDR to allow multiple instances of this */
    int reuse = 1;
    if(setsockopt(recver->m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
        LOG("failed to set SO_REUSEADDR ! \n");
        free(recver);
        return NULL;        
    }    

    /* Bind to the proper port number with the IP address specified as INADDR_ANY.*/
    struct sockaddr_in m_localaddr;
    memset((char *) &m_localaddr, 0, sizeof(m_localaddr));
    m_localaddr.sin_family = AF_INET;
    m_localaddr.sin_port = htons(multicast_port);
    m_localaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    if(bind(recver->m_socket, (struct sockaddr*)&m_localaddr, sizeof(m_localaddr)) < 0) {
        LOG("failed to bind datagram socket ! \n");
        free(recver);
        return NULL;
    }    

    /* Join the multicast group on the local interface */
    /* Note that this IP_ADD_MEMBERSHIP option must be called for each local interface over which the multicast */
    /* datagrams are to be received. */
    struct ip_mreq group;
    memset((char *)&group, 0, sizeof(group));    
    group.imr_multiaddr.s_addr = inet_addr(multicast_addr);    
    group.imr_interface.s_addr = htonl(INADDR_ANY);  
    if(setsockopt(recver->m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0) {
        LOG("failed to adding multicast group !\n ");
        free(recver);
        return NULL;
    }

    LOG("mc_recver_create success !\n");

    return recver;
}

void mc_recver_destroy(mc_recver_t * recver)
{
    if(recver == NULL) {
        return;
    }

    shutdown(recver->m_socket,SHUT_RDWR); 
    close(recver->m_socket);
    free(recver);

    LOG("mc_recver_destroy success !\n");
}

int mc_recver_recv(mc_recver_t * recver, unsigned char *buffer, int len)
{
    if(recver == NULL) {
        return -1;
    }
    int socketlen = 0;
    return recvfrom(recver->m_socket, buffer, len, 0, (struct sockaddr *)&recver->m_localaddr, &socketlen);
}


