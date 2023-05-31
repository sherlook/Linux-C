/**
 * fork()
 * wait()
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

#define LEFT  30000000
#define RIGHT 30000200

int main()
{
    int i,j,mark;
    pid_t pid;
    for(i=LEFT; i<=RIGHT; i++)
    {   
        pid = fork();
        if(pid < 0)
        {
            perror("fork() fail\n");
            exit(1);
        }
        if(pid == 0)
        {   
            //printf("i:[%d],pid:[%d]\n",i,getpid());
            mark = 1;
            for(j=2; j<i/2; j++)
            {   
                
                if(i%j == 0)
                {
                    mark = 0;
                    break;
                }
            }
            if(mark)
            {
                printf("%d is a prime.\n", i);
            }
            // 子进程sleep(1000);  父进程先结束
            //sleep(1000);
            /**
             * 结束子进程，如果不结束，子进程会进入到下一个循环（回到子进程的for循环中），继续创建进程
            */
            exit(0);
        }
    }

    int st;
    for(i=LEFT; i<=RIGHT; i++)
    {
        wait(&st);
    }


    exit(0);
}