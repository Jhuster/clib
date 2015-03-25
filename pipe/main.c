#include <stdio.h>
#include "pipe.h"

typedef struct _user {    
    char * m_name;
    int m_age;
    int m_sex;
}user_t;

void put_user_list(pipe_t *pipe) {

    user_t user1;
    user1.m_name = "Juster";
    user1.m_age  = 27;
    user1.m_sex  = 1;

    user_t user2;
    user2.m_name = "Wang";
    user2.m_age  = 22;
    user2.m_sex  = 0;

    user_t user3;
    user3.m_name = "David";
    user3.m_age  = 36;
    user3.m_sex  = 1;

    pipe_put(pipe,&user1);
    pipe_put(pipe,&user2);
    pipe_put(pipe,&user3);
}

int main( int argc, char *argv[] ) {

    pipe_t user_pipe;

    if( pipe_open(&user_pipe,sizeof(user_t),PIPE_NO_BLOCKING) != PIPE_SUCCESS ) {
        printf("Failed to open pipes !\n");
        return -1;
    }

    printf("User pipe open success !\n");

    put_user_list(&user_pipe);

    user_t user;
    while( pipe_get(&user_pipe,&user) != PIPE_FAILURE ) {
        printf("user %s, age=%d,sex=%d !\n",user.m_name,user.m_age,user.m_sex);
    }

    pipe_close(&user_pipe);

    printf("User pipe close success !\n");

    return 0;
}