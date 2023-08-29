#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "anytimer.h"

void func1(char * str)
{
    printf("%s\n", str);
}

static void alrm_handler()
{
    printf("1s\n");
    alarm(1);
}


int main()
{
    // 
    char s[10] = "5";
    int ret = 0;

    //定时一个5s任务
    ret = anytimer_addjob(5, func1, s);
    printf("id:%d\n", ret);

    ret = anytimer_addjob(2, func1, "2");
    printf("id:%d\n", ret);

    ret = anytimer_addjob(3, func1, "3");
    printf("id:%d\n", ret);

    while(1)
        pause();
}
