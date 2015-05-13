#include "stdio.h"
/*#include "stdarg.h "*/

#define GUI_DEBUG_PRINTF(format, args...)                                       \
    do                                                                          \
    {                                                                           \
        printf("[%s]:[%s]:[%d] "format,__FILE__, __FUNCTION__,__LINE__,##args); \
    } while (0)


#define printf(args...) fprintf(stderr, args)



void main()
{
    char aaa[] = "sbcisco";
/*    printf("hello world\n""aaa %d\n""%s %d", 12,"aaabbc\n", 12);*/
/*fprintf(stderr, "%s", aaa);*/
printf( "%s\n", aaa);

/*    GUI_DEBUG_PRINTF("hello new world\n");*/
}
