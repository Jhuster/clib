#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MULTICAST_ADDR "239.255.255.250"
#define MULTICAST_PORT 9600

#define PACKET_SIZE 1024

#define LOG(format,...) printf(format,##__VA_ARGS__)

int multicast_sender(int argc, char **argv) 
{
    /* create socket handler*/
    int msocket = -1;
    if((msocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {     
        LOG("failed to create socket !\n");
        close(msocket);   
        return -1;
    }

    /* Initialize the group socket addr */
    struct sockaddr_in group_addr;
    memset((char *)&group_addr, 0, sizeof(group_addr));
    group_addr.sin_family = AF_INET;
    group_addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    group_addr.sin_port = htons(MULTICAST_PORT);

    /* Disable loopback so you do not receive your own datagrams */
    char enabled = 1;
    if(setsockopt(msocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&enabled, sizeof(enabled)) < 0) {
        LOG("failed to disabling the loopback !\n");
        close(msocket);
        return -1;
    }
    LOG("disabling the loopback success ! \n");    

    /* Set local interface for outbound multicast datagrams. */
    struct in_addr interface;
    interface.s_addr = htonl(INADDR_ANY);  
    if(setsockopt(msocket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&interface, sizeof(interface)) < 0) {
        LOG("failed to set local interface ! \n");
        close(msocket);
        return -1;
    }    
    LOG("setting the local interface success ! \n");

    /* send multicast data to multicast addr */
    char *buffer = "Hello Wolrd";
    while(1) {
        if(sendto(msocket, buffer, strlen(buffer)+1, 0, (struct sockaddr*)&group_addr, sizeof(group_addr)) < 0) {
            LOG("failed to send multicast data ! \n");                        
        }
        else {
            LOG("send multicast data success ! \n");
        }   
        sleep(2);     
    }

    close(msocket);

    return 0;
}

int multicast_receiver(int argc, char **argv) 
{                
    /* create socket handler*/
    int msocket = -1;
    if((msocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {     
        LOG("failed to create socket !\n");   
        return -1;
    }

    /* Enable SO_REUSEADDR to allow multiple instances of this */
    int reuse = 1;
    if(setsockopt(msocket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
        LOG("failed to set SO_REUSEADDR ! \n");
        close(msocket);
        return -1;
    }    
    LOG("setting SO_REUSEADDR success ! \n");

    /* Bind to the proper port number with the IP address specified as INADDR_ANY.*/
    struct sockaddr_in localaddr;
    memset((char *) &localaddr, 0, sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(MULTICAST_PORT);
    localaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    if(bind(msocket, (struct sockaddr*)&localaddr, sizeof(localaddr)) < 0) {
        LOG("failed to bind datagram socket ! \n");
        close(msocket);
        return -1;
    }    
    LOG("binding datagram socket success, port = %d ! \n",MULTICAST_PORT);

    /* Join the multicast group on the local interface */
    /* Note that this IP_ADD_MEMBERSHIP option must be called for each local interface over which the multicast */
    /* datagrams are to be received. */
    struct ip_mreq group;
    memset((char *)&group, 0, sizeof(group));    
    group.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);    
    group.imr_interface.s_addr = htonl(INADDR_ANY);  
    if(setsockopt(msocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0) {
        LOG("failed to adding multicast group !\n ");
        close(msocket);
        return -1;
    }
    LOG("adding multicast group success ! \n");
  
    /* recv multicast data from multicast addr */
    char packet[PACKET_SIZE];
    for(;;) {         
        int socketlen = 0;
        memset(packet,0,PACKET_SIZE);         
        int nbytes = recvfrom(msocket, packet, PACKET_SIZE, 0, (struct sockaddr *)&localaddr, &socketlen);
        if(nbytes < 0) {      
            LOG("failed to recv multicast data ! \n");
            continue;            
        }
        LOG("recv: %s \n",packet);    
    }

    return 0;
}

void usage(char *program) 
{
    LOG("Usage:\n%s [-c as client] [-s as server] [-h help] \n",program);
    LOG("example:\n");
    LOG("\t%s -s \n",program);
}

int main(int argc, char **argv) 
{
    int c;

    if(argc < 2) {
        usage(argv[0]);
        return -1;
    }

    while((c = getopt(argc, argv, "csh?")) != -1) {
        switch (c) {
        case 'c': return multicast_receiver(argc,argv);
        case 's': return multicast_sender(argc,argv);                
        default:
            usage(argv[0]);
            return 1;
        }
    }

    return 0;
}