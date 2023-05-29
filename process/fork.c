/**
 * fork()创建进程
 * 
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    pid_t pid = 0;
    printf("[%d]begin\n", getpid());
    
    /* !!! */
    fflush(NULL);

    pid = fork();
    if(pid < 0)
    {
        perror("fork() fail\n");
        exit(1);
    }
    if(pid == 0)
    {
        // 子进程中执行
        printf("[%d]child process \n", getpid());
    }
    else
    {
        // 父进程中执行
        printf("[%d]parent process \n", getpid());
    }
    printf("[%d]end\n", getpid());
    exit(0);
}