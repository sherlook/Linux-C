/**
 * 
 * 让一个数自加5m(用time()实现五秒钟)
 *   
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

int main()
{
    int64_t count = 0;
    time_t end = time(NULL) + 5;

    while(time(NULL) <= end)
    {
        count++;
    }

    printf("%ld\n", count);
    exit(0);
}