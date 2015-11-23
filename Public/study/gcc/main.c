
#include <stdio.h>
#if 0
#include <stdio.h>
#include <apue.h>
       int
       main(void)
       {
           char    *ptr;
           size_t      size;
           if (chdir("/usr/spool/uucppublic") < 0)
               err_sys("chdir failed");
           ptr = path_alloc(&size);    /* our own function */
           if (getcwd(ptr, size) == NULL)
               err_sys("getcwd failed");
           printf("cwd = %s\n", ptr);
exit(0); 
}
#endif

typedef  enum
_CCSP_SB_BLOCKEDSITE_BLOCKMETHOD
{
    CCSP_SB_BLOCKEDSITE_BLOCKMETHOD_URL    = 1,
    CCSP_SB_BLOCKEDSITE_BLOCKMETHOD_Keyword,
}
    CCSP_SB_BLOCKEDSITE_BLOCKMETHOD;
//file_read_test
int main(int argc, char *argv[])
{
    int var;
    printf("%d  %d\n", CCSP_SB_BLOCKEDSITE_BLOCKMETHOD_URL, CCSP_SB_BLOCKEDSITE_BLOCKMETHOD_Keyword);
/*    printf("%x\n",atoi(argv[1]));*/
    if(1)
    ;
    printf("aaa\n");
}

/*int main()*/
/*{*/
/*str*/
/*}*/

#if 0
int main(void)
{
    int c;
    FILE *fd = fopen("/workspace/guoding/nas/5k6k/EDA/src/application/generic/main.c", "r");
    fseek(fd, 0, SEEK_END);
    long int ret = ftell(fd);
    printf("size = %d\n", ret);
    fseek(fd, 0, SEEK_SET);
    while((c=fgetc(fd))!=EOF)
    {
        printf("%c", c);
    }
    fclose(fd);
}
#endif

#if 0
int main(void)
{
int  i;

//ÕâÊÇ×Ö·ûÊý×é¸³³õÖµµÄ·½·¨£¬Î¬Êý£º4£Ø2
/*int *abc;*/
/**abc = 100;*/
char cArr[][2] = {{'Q','U'},
                 {'A','N'},
                 {'X','U'},
                 {'E','\0'}};
//ÕâÊÇ×Ö·û´®Êý×é¸³³õÖµµÄ·½·¨£¬Î¬Êý£º4£Ø3
char sArr[][3] = {"qu","an","xu","e"};

//ÓÃsizeof()Çó³¤¶È
for (i=0; i<4; i++) {
    printf("%d: %d  %d\n", i, sizeof(cArr[i]), sizeof(sArr[i]));
    printf("[%s] [%s]\n", cArr[i], sArr[i]);
}

return 0;
}
#endif
