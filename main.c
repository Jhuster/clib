#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define LOG(format, ...) printf(format,##__VA_ARGS__)

void usage(char *program) 
{
    LOG("Usage:\n%s [-i interface [null/eth0/eth1...]] [-p listenport] [-h help] \n", program);
    LOG("example:\n");
    LOG("\t%s -i eth0 -p 6000 \n",program);
}

int main(int argc, char *argv[])
{
    int c;
    char interface[64];
    int listen_port;

    // argv[0] == program name
    usage(argv[0]);

    LOG("You have input %d params\n",argc-1); 

    //xyz: 单个选项，例如： -h
    //x:   如果x存在，则必须跟一个参数(带空格)，例如： -p 6000
    //x::  如果x存在，则必须跟一个参数（不带空格），例如： -ieth0
    while ((c = getopt(argc, argv, "i:p:h?")) != -1) {
        switch (c) {
        case 'i':
            strcpy(interface , optarg);  
            LOG("interface %s \n",interface);
            break;
        case 'p':
            listen_port = atoi(optarg);
            LOG("listen_port %d \n", listen_port); 
            break;
        case 'h':
        case '?':
            LOG("Need help ?\n");
            break;        
        default:
            usage(argv[0]);
            return 1;
        }
    }

    return 0;
}
