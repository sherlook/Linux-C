/*
 * sig***set()函数例子
 *         sigemptyset();sigfillset();sigaddset();sigdelset();
 *          sigprocmask()  
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
    // 在程序未结束的情况下， 收到一个SIGINT信号， 执行 int_handler()
    signal(SIGINT, *int_handler);

    sigset_t set,oset, saveset;

    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    
    // 保存之前的信号状态
    sigprocmask(SIG_UNBLOC, &set, &saveset);

    for(int j=0; j< 1000; j++)
    {
        sigprocmask(SIG_BLOCK, &set, &oset);
        for(int i=0; i<5; i++)
        {
            write(1, "*", 1);
            sleep(1);
        }
        write(1, "\n", 1);
        sigprocmask(SIG_SETMASK, &oset, NULL);
    }

    // 恢复之前的信号状态
    sigprocmask(SIG_SETMASK, &saveset, NULL);

    exit(0);
}
