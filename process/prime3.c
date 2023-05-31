/**
 * fork()
 * wait()
 * N个进程 交叉分配     
 *      0进程永远筛不到质数
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

#define LEFT  30000000
#define RIGHT 30000200
#define N     3

int main()
{
    int i, j, mark, n;
    pid_t pid;

    for(n=0; n<N; n++)
    {
        pid = fork();
        if(pid<0)
        {
            perror("fork fail\n");
            exit(1);
        }
        if(pid == 0)
        {
            for(i=LEFT+n; i<=RIGHT; i+=N)
            {   
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
                    //
                    printf("[%d]%d为质数\n", n, i);
                }
            }
            exit(0);
        }
    }

    for(n=0; n<N; n++)
    {
        wait(NULL);
    }


    exit(0);
}