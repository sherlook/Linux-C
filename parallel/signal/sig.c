/*
 * signal()函数例子
 *      在标准输入中输入 "*"
 *      用ctrl+c信号打断程序  SIGINT
 * 
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<signal.h>


static void int_handler(int s)
{
    write(1, "!", 1);
}


int main()
{
    
    // 给SIGINT信号设置 SIG_IGN的处理方式（忽略信号）
    // signal(SIGINT, SIG_IGN);

    // 在程序未结束的情况下， 收到一个SIGINT信号， 执行 int_handler()
    signal(SIGINT, *int_handler);

    for(int i=0; i<10; i++)
    {
        write(1, "*", 1);
        sleep(1);
    }

    exit(0);
}