
/*static char abc[] = "aaaaaaaaa";*/
#define abc "aaa%daaaaaa"

void main()
{
    char aaa[100];
/*    printf("%s\n", abc);*/
    sprintf(aaa, abc, 123);
    printf("%s\n",aaa);
    double Dvar;
    Dvar = (double)800000/1024/1024;
    printf("%f\n",Dvar);
}
