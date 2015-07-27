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
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "broadcaster.h"

#define LOG(format,...) printf(format,##__VA_ARGS__)

broadcaster_t * broadcaster_create(int workmode, int port) 
{
    if( workmode != BROADCASTER_WORKMODE_SENDER && workmode != BROADCASTER_WORKMODE_RECVER ) {
        LOG("broadcaster_create param invalid !\n");
        return NULL;
    }

    broadcaster_t * broadcaster = (broadcaster_t *)malloc(sizeof(broadcaster_t));    
    memset(broadcaster,0,sizeof(broadcaster_t));    

    broadcaster->m_workmode = workmode;

    if ((broadcaster->m_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {             
        free(broadcaster);
        LOG("broadcaster_create create socket failed !\n");
        return NULL;  
    }  

    int opt=1;      
    int ret = setsockopt(broadcaster->m_socket, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));  
    if( ret < 0 ) {    
        free(broadcaster);
        LOG("broadcaster_create setsockopt SO_BROADCAST failed !\n");
        return NULL;  
    }  

    ret = setsockopt(broadcaster->m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)); 
    if( ret < 0 ) {    
        free(broadcaster);
        LOG("broadcaster_create setsockopt SO_REUSEADDR failed !\n");
        return NULL;  
    }      

    bzero(&broadcaster->m_addr, sizeof(struct sockaddr_in));  
    broadcaster->m_addr.sin_family=AF_INET;  
    broadcaster->m_addr.sin_addr.s_addr=htonl(INADDR_BROADCAST);  
    broadcaster->m_addr.sin_port=htons(port);   

    if( bind(broadcaster->m_socket,(struct sockaddr *)&(broadcaster->m_addr), sizeof(struct sockaddr_in)) == -1) {     
        free(broadcaster);
        LOG("broadcaster_create bind sockopt failed !\n");
        return NULL;    
    }      

    LOG("broadcaster_create success !\n");

    return broadcaster;
}

void broadcaster_destroy(broadcaster_t * broadcaster) 
{
    if( broadcaster == NULL || broadcaster->m_socket == -1 ) {
        return;
    }

    shutdown(broadcaster->m_socket,SHUT_RDWR); 
    close(broadcaster->m_socket);
    broadcaster->m_socket = -1;
    free(broadcaster);

    LOG("broadcast_session_destroy success !\n");
}

void broadcaster_interrupt(broadcaster_t * broadcaster)
{
    if( broadcaster == NULL || broadcaster->m_socket == -1 ) {
        return;
    }

    shutdown(broadcaster->m_socket,SHUT_RDWR); 

    LOG("broadcast_session broadcaster_interrupt !\n");
}

int broadcaster_send_packet(broadcaster_t * broadcaster, unsigned char *buffer, int len) 
{
    if( broadcaster == NULL || broadcaster->m_socket == -1 || broadcaster->m_workmode != BROADCASTER_WORKMODE_SENDER ) {
        return -1;
    }

    int ret = sendto(broadcaster->m_socket, buffer, len, 0, (struct sockaddr*)&broadcaster->m_addr, sizeof(broadcaster->m_addr));  
    if( ret < 0 ) {    
        LOG("broadcast_send_packet failed,error=%d\n",ret);
        return -1;  
    }

    return 0;
}

int broadcaster_recv_packet(broadcaster_t * broadcaster, unsigned char *buffer, int len) 
{
    if( broadcaster == NULL || broadcaster->m_socket == -1 || broadcaster->m_workmode != BROADCASTER_WORKMODE_RECVER ) {
        return -1;
    }

    int nbytes;
    int ret = recvfrom(broadcaster->m_socket, buffer, len, 0, (struct sockaddr*)&broadcaster->m_addr,(socklen_t*)&nbytes);  
    if( ret < 0 ) {
        LOG("broadcast_recv_packet failed,error=%d\n",ret);
        return -1;
    }

    return nbytes;    
}