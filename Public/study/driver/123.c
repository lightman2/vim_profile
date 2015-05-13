#include <stdio.h>
#include <time.h>

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
/*#include <linux/spi/spidev.h>*/


int main(int argc, char *argv[])
{
int ret = 0;
	int fd;
    FILE *ft;
    time_t tim;
    struct tm *at;
    char now[80];
/*time*/
/*    time(&tim);*/
/*    at=localtime(&tim);    */
/*    strftime(now,79,"%Y-%m-%d\n%H:%M:%S\n",at);*/
/*    while(1)*/
/*    {*/
/*        printf("helloworld now : %s\n", now);*/
/*    }*/

/*strtol*/
/*    char    *endptr;*/
/*    int number=0;*/
/*    if(NULL!=argv[1])*/
/*    {*/
/*        number = strtol(argv[1],&endptr,10 );*/
/*    }*/
/*    printf("number*2 =%d", number*2);*/

/*scanf*/
/*    char *output;*/
/*    output = malloc(100);*/
/*    printf("\n\nPlease enter a label for the chart: ");    */
/*    scanf("%100s", output ); */
/*    fflush(stdin);  |+ flush the input stream in case of bad input +| */
/*    printf("%s\n", output);    */

/*getenv*/
/*    char env[1000];*/
/*    sprintf(env,"%s",getenv("PATH"));*/
/*    printf("%s", env);*/

/*sprintf*/
    char secure_os_name[50], buf[50];
    sprintf(buf, "111111111111122222222222222333333334444444445555555555");
    if(*((unsigned int*)buf) == '1'<<24|'1'<<16|'1'<<8|'1' )
    {
        sprintf(secure_os_name, "%s", (buf+10));
    }
    else
    {
        sprintf(secure_os_name, "%s", "");
    }
    printf("%s\n",secure_os_name);
	return ret;
}
