//5sec_sig.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

//使用 信号 计算当前单核 累加 的速度

volatile static int loop = 1;

static void handler(int sig){
    loop = 0;
}

int main()
{
    int64_t count = 0;

    // signal()需要卸载alarm()前面，如果alarm()在前，到signal()可能执行了很长时间，超过时钟，但程序不知道跳转到signal_handler去执行
    signal(SIGALRM,handler);
    alarm(5);

    // while(loop){
    //     count++;
    // }

    // printf("count %ld\n",count);

    exit(0);
}