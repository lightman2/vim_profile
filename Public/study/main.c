#include "stdio.h"

/*void main()*/
/*{*/
/*    printf("hello world!");*/
/*}*/
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
