/*
    time()

    
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define Maxsize 1024

int main()
{   
    int ret;
    char timebuf[Maxsize];

    // time_t
    time_t tm;
    tm = time(NULL);

    // time_t * ptm;
    // time(ptm);

    // time_t tm3;
    // time(&tm);

    printf("time: %ld\n", tm);

    // time_t  ->  struct tm
    struct tm * pstm;
    pstm = localtime(&tm);

    printf("year: %d, month: %d, day: %d\n", pstm->tm_year+1900, pstm->tm_mon+1, pstm->tm_mday);

    ret = strftime(timebuf, Maxsize, "%Y %m %d", pstm);

    puts(timebuf);

    return 0;

}