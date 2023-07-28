/**
 * 
 * 
 * 
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{

    alarm(5);

    // 无法设置多任务计时器
    alarm(10);
    alarm(5);    
    alarm(1);


    while(1)
        pause();
        
    exit(0);
}