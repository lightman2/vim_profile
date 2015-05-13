#include <stdlib.h>    
#include <stdio.h>    
#include <pthread.h>    
#include <errno.h>    
     
static void* pthread_func_1 (void*);    
static void* pthread_func_2 (void*);    
    
  pthread_t pt_1 = 0;    
  pthread_t pt_2 = 0;    
static void* applicationcheck();
int main (int argc, char** argv)    
{    
  int ret = 0;    
     
/*  ret = pthread_create (&pt_1, NULL, pthread_func_1, NULL);    */
  ret = pthread_create (&pt_1, NULL, applicationcheck, NULL);    

  if (ret != 0)    
  {    
     perror ("pthread_1_create");    
  }    
    
  ret = pthread_create (&pt_2, NULL, pthread_func_2, NULL);    
/*  if (ret != 0)    */
/*  {    */
/*      perror ("pthread_2_create");    */
/*   }    */
/*     */
   printf("line = %d\n", __LINE__);
   pthread_join (pt_1, NULL);    
   printf("line = %d\n", __LINE__);
   pthread_join (pt_2, NULL);    
   printf("line = %d\n", __LINE__);
     
/*   return 0;    */
/*  while(1)*/
/*  {*/
/*      sleep(1);*/
/*  }*/
}    
     
static void* pthread_func_1 (void* data)    
{    
   int i = 0;    
       
   for (; i < 6;)    
   {    
     printf ("This is pthread1!/n");    
     
/*     if (i == 2)    */
/*     {    */
/*       pthread_exit (0);    */
/*     }    */
/*     */
/*     sleep (1);    */
   }    
  
   return NULL;  
}    
     
static void* pthread_func_2 (void* data)    
{    
   int i = 0;    
     
/*   for (; i < 3; i++)    */
/*   {    */
/*     printf ("This is pthread2!/n");    */
/*   }    */
/*     */
   sleep(5);
   printf("killstart!\n");
   pthread_cancel(pt_1);
   printf("killend!\n");
   pthread_exit (0);    
   printf("line = %d\n", __LINE__);
  
   return NULL;  
}    



static void* applicationcheck()
{
    while(1)
    {
        sleep(2);
   printf("line = %d\n", __LINE__);
        printf("running application check\n");
   printf("line = %d\n", __LINE__);
    }
}
