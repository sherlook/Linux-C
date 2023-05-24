/*
 * getenv()获取环境变量
 * 
*/
#include<stdio.h>
#include<stdlib.h>

int main()
{
    char * penv;
    penv = getenv("PATH");
    puts(penv);

    //停
    getchar();

    exit(0);
}