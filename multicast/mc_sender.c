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
#include <unistd.h>
#include <sys/socket.h>
#include "mc_sender.h"

#define LOG(format, ...) printf(format, ##__VA_ARGS__)

mc_sender_t *mc_sender_create(char *multicast_addr, int multicast_port)
{
    mc_sender_t *sender = (mc_sender_t *) malloc(sizeof(mc_sender_t));

    /* create socket handler*/
    if ((sender->m_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {     
        LOG("failed to create socket !\n");
        free(sender);
        return NULL;
    }

    /* Initialize the group socket addr */
    memset((char *)&sender->m_multicast_addr, 0, sizeof(sender->m_multicast_addr));
    sender->m_multicast_addr.sin_family = AF_INET;
    sender->m_multicast_addr.sin_addr.s_addr = inet_addr(multicast_addr);
    sender->m_multicast_addr.sin_port = htons(multicast_port);

    /* Enable/Disable loopback so you can/not receive your own datagrams */
    char enabled = 1;
    if (setsockopt(sender->m_socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&enabled, sizeof(enabled)) < 0) {
        LOG("failed to config the loopback !\n");
        free(sender);
        return NULL;
    }

    /* Set local interface for outbound multicast datagrams. */
    struct in_addr interface;
    interface.s_addr = htonl(INADDR_ANY);  
    if (setsockopt(sender->m_socket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&interface, sizeof(interface)) < 0) {
        LOG("failed to set local interface ! \n");
        free(sender);
        return NULL;
    }    

    LOG("mc_sender_create success !\n");

    return sender;
}

void mc_sender_destroy(mc_sender_t * sender)
{
    if (sender == NULL) {
        return;
    }

    shutdown(sender->m_socket,SHUT_RDWR); 
    close(sender->m_socket);
    free(sender);

    LOG("mc_sender_destroy success !\n");
}

int mc_sender_send(mc_sender_t * sender, unsigned char *buffer, int len)
{
    if (sender == NULL) {
        return -1;
    }
    return sendto(sender->m_socket, buffer, len, 0, (struct sockaddr*)&sender->m_multicast_addr, sizeof(sender->m_multicast_addr));
}