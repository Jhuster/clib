#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mc_sender.h"
#include "mc_recver.h"

#define LOG(format, ...) printf(format, ##__VA_ARGS__)

/* any address between 224.0.0.0 and 239.255.255.255 */
#define MULTICAST_ADDR "239.255.255.250"  
#define MULTICAST_PORT 9600

#define PACKET_SIZE 1024

int multicast_sender(int argc, char **argv) 
{
    mc_sender_t *sender = mc_sender_create(MULTICAST_ADDR, MULTICAST_PORT);
    if (sender == NULL) {
        return -1;
    }

    char *buffer = "Hello Wolrd";
    while (1) {
        if (mc_sender_send(sender, buffer, strlen(buffer) + 1) < 0) {
            LOG("failed to send multicast data ! \n");                        
        } else {
            LOG("send multicast data success ! \n");
        }   
        sleep(2);
    }

    mc_sender_destroy(sender);

    return 0;
}

int multicast_receiver(int argc, char **argv) 
{
    mc_recver_t *recver = mc_recver_create(MULTICAST_ADDR, MULTICAST_PORT);
    if (recver == NULL) {
        return -1;
    }

    char packet[PACKET_SIZE];
    while(1) {         
        if (mc_recver_recv(recver, packet, PACKET_SIZE) < 0) {      
            LOG("failed to recv multicast data ! \n");
            continue;            
        }
        LOG("recv: %s \n", packet);    
    }

    mc_recver_destroy(recver);

    return 0;
}

void usage(char *program) 
{
    LOG("Usage:\n%s [-c as client] [-s as server] [-h help] \n", program);
    LOG("example:\n");
    LOG("\t%s -s \n", program);
}

int main(int argc, char **argv) 
{
    int c;

    if (argc < 2) {
        usage(argv[0]);
        return -1;
    }

    while((c = getopt(argc, argv, "csh?")) != -1) {
        switch (c) {
        case 'c': return multicast_receiver(argc, argv);
        case 's': return multicast_sender(argc, argv);                
        default:
            usage(argv[0]);
            return 1;
        }
    }

    return 0;
}