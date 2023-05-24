/**
 * 今天是2023/5/16 求100天后的日期
 * 方法：
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define BUFFSIZE 1024

int main()
{
    time_t tm;
    struct tm * ptm;
    char timebuf[BUFFSIZE];
    
    time(&tm);

    ptm = localtime(&tm);

    ptm->tm_mday += 100;

    mktime(ptm);
    // mktime 会把 ptm 转为 tm类型；
    //       先判断ptm格式是否合法，如果不合法改为合法，  mktime()没有const

    strftime(timebuf, BUFFSIZE, "100years after : %Y-%m-%d", ptm);   
    puts(timebuf);
    
    exit(0);
}























