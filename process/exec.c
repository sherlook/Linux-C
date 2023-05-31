/**
 * exec()族函数
 * date +%s
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    puts("Begin");

    /****/
    fflush(NULL);

    // 用一个新的进程替换当前的进程
    execl("/usr/bin/ls", "ls", "-a", NULL);

    puts("dd end");

    //如果执行到perror()一定错了
    perror("exec()\n");
    
    puts("End");

    exit(0);
}   
