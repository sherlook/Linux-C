/**
 * 刷新缓冲区fflush()
 * 不刷新则输出还在缓冲区中，没有写入到stream，
 * 全缓冲
 * 行缓冲
 * 无缓冲 stderr
*/


#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("before while()");
    fflush(stdout);

    while(1);

    printf("after while()");
    fflush(NULL);
    
    exit(0);
}