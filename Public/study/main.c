#include "stdio.h"

/*void main()*/
/*{*/
/*    printf("hello world!");*/
/*}*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigHandle(int sig) {
    switch(sig) {
    case SIGINT:
        printf("sigHandle: %d, SIGINT\n", sig);
        break;
    default:
        printf("sigHandle: %d, OTHER\n", sig);
        break;
    }
    exit(1);
}

int main() {
    /* signal(SIGINT, sigHandle); */
   /* execl("/bin/ls","", NULL); */
    /* signal(SIGINT, SIG_IGN); */
    //for(;;) {
//    while(1){


#define aaa
#ifndef aaa
    printf("udef aaa\n");
#endif
        int *a= malloc(100);
        a[1] = 100;
        a[2] = 101;
        int ret=0;
        ret= fork();
        printf("fork ret = %d\n", ret);
        printf("11\n");
        system("ls -alt");
        usleep(1000000);
//    }
    //usleep(1000000);
    return 0;
}
#if 0
static int aaa3;
static int aaa2;
static int aaa;
int main() { 
    char name[64]; 
    int bbb;
    printf("%p\n", name);  // Print address of buffer. 
    printf("%d\n", aaa);  // Print address of buffer. 
    puts("What's your name?"); 
    gets(name); 
    printf("Hello, %s!\n", name); 
    return 0; 
}
#endif
