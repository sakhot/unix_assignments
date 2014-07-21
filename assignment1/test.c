#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_init = 5;
int global_uninit;

int foo(int **malloc_here)
{
    int local_x, local_y=3;
    printf("\n malloc_here=%p val=%p", &malloc_here, *malloc_here);
    *malloc_here = (int*) malloc(sizeof(int));
    printf("\n local_x=%d, local_y=%d", local_x, local_y);
    printf("\n before initializing malloc_here=%d", **malloc_here);
    **malloc_here = 99;
    printf("\n after initializing malloc_here=%d", **malloc_here);
    return 0;
}

void open_file()
{
    FILE *fp = fopen("test.c","r");
    char buff[51];
    fread(buff, 1,50,fp);
    buff[50] = '\0';
    printf("\n -----FILE contents START-----");
    printf("\n FILE contents=buff=%s", buff);
    printf("\n -----FILE contents END-----\n");
    fclose(fp);
}

int main()
{
 int *malloc_var=NULL;
 int a = 8;
 printf("\n Hello world global init=%d uninit=%d", global_init, global_uninit);
 printf("\n malloc_var=%d addr=%p",(int)malloc_var, &malloc_var);

 open_file();
 foo(&malloc_var);
 printf("\n Main malloce_var val=%d a=%d\n", *malloc_var, a);
 sleep(10);
 free(malloc_var);
 return 0;
}
