#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <memory.h>
#include "broadcaster.h"

#define LOG(format,...) printf(format,##__VA_ARGS__)

#define DEFAULT_PORT 8000

static broadcaster_t * gBroadcaster = NULL;

volatile int g_loop_exit = 0;
void exit_callback(int signum) {
    g_loop_exit = 1;
    if( gBroadcaster != NULL ) {
        broadcaster_interrupt(gBroadcaster);
    }    
}

int send_loop(int port) 
{
    char * message = "Hello World!";

    gBroadcaster = broadcaster_create(port);
    if(gBroadcaster==NULL) {
        return -1;
    }

    while(!g_loop_exit) {
        sleep(1);        
        if( broadcaster_send_packet(gBroadcaster,message,strlen(message)) >= 0 ) {
            LOG("send %s \n",message);
        }
    }

    broadcaster_destroy(gBroadcaster);

    LOG("Ok,send_loop exit !\n");

    return 0;
}

int recv_loop(int port) 
{
    char message[20];

    gBroadcaster = broadcaster_create(port);
    if(gBroadcaster==NULL) {
        return -1;
    }

    while(!g_loop_exit) {
        memset(message,0,20);
        if( broadcaster_recv_packet(gBroadcaster,message,20) >= 0 ) {
            LOG("recv %s \n",message);
        }        
    }

    broadcaster_destroy(gBroadcaster);

    LOG("Ok,recv_loop exit !\n");

    return 0;
}

void usage(char *program) {
    LOG("Usage:\n%s [-t work mode [send/recv]] [-p port] [-h help] \n",program);
    LOG("example:\n");
    LOG("\t%s -t send -p 8000 \n",program);
}

int main(int argc, char *argv[])
{
    int c;    
    int port = DEFAULT_PORT;
    char workmode[20];

    if(argc < 3) {
        usage(argv[0]);
        return 1;
    }

    signal(SIGINT,exit_callback);

    //xyz: 单个选项，例如： -h
    //x:   如果x存在，则必须跟一个参数(带空格)，例如： -p 6000
    //x::  如果x存在，则必须跟一个参数（不带空格），例如： -ieth0
    while ((c = getopt(argc, argv, "t:p:h")) != -1) {
        switch (c) 
        {
        case 't':
            strcpy(workmode , optarg);  
            LOG("workmode %s \n",workmode);
            break;
        case 'p':
            port = atoi(optarg);
            LOG("port %d \n",port); 
            break;
        case 'h':
        default:
            usage(argv[0]);
            return 1;
        }
    }

    if( strcmp(workmode,"send") == 0) {
        send_loop(port);
    }
    else if(strcmp(workmode,"recv") == 0) {
        recv_loop(port);
    }
    else {
        usage(argv[0]);
        return 1;
    }

    LOG("\nOk,exit program now !\n");

    return 0;
}
