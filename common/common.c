#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

#define LOG(format, ...) printf(format,##__VA_ARGS__)

#define TIME_FORMAT "%Y%m%d%H%M%S"

void get_time_str(char *timestr, int len) 
{
    time_t now;
    time(&now);
    strftime(timestr, len, TIME_FORMAT, localtime(&now));    
}

unsigned int get_time_in_ms()
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0)
        return 0;
    return tv.tv_sec * 1000 + tv.tv_usec/1000;
}

// convert 2 bytes from one endian to another endian
#define CONVERT(x)  (unsigned short)(((x&0xff00)>>8)|((x&0x00ff)<<8))

typedef union {
    unsigned short value;   // short = 2 bytes
    unsigned char  byte[2]; // char  = 1 bytes
} ubytes_t;

int is_big_endian() 
{
    ubytes_t test;
    test.value = 0x1234;
    //test.value = CONVERT(test.value); //test convert
    if( test.byte[0] == 0x12 && test.byte[1] == 0x34 ) {        
        return 1;
    } else if( test.byte[0] == 0x34 && test.byte[1] == 0x12 ) {        
        return 0;
    }

    return -1;
}

static volatile int g_loop_exit = 0;
static void exit_callback(int signum) 
{
    g_loop_exit = 1;
    LOG("\nOk,exit program now !\n");
}

int main(int argc, char *argv[])
{
    char timestr[20];
    get_time_str(timestr,20);

    LOG("%s\n", timestr);

    int ret = is_big_endian();
    if (ret == 1) {
        LOG("big endian! \n");
    } else if(ret == 0) {
        LOG("little endian !\n");
    } else {
        LOG("unknow error !\n");   
    }

    unsigned int start = get_time_in_ms();
    usleep(50 * 1000);
    unsigned int end = get_time_in_ms();

    LOG("time spend %d ms \n", end-start);

    signal(SIGINT, exit_callback);
    LOG("please enter ctrl + c to exit !\n");

    while (!g_loop_exit) {
        usleep(1000 * 1000);
    }

    return 0;
}
