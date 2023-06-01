/**
 * fork()
 * exec()
 * wait()
 * 
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>    

int main()
{   
    pid_t pid;
    puts("Bgein");

    fflush(NULL);

    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    if(pid == 0)
    {
        execl("/usr/bin/date", "date", "+%s", NULL);
        // execl("/bin/sh", "sh", "-c", "date+%s", NULL);
        perror("exec()");
        exit(1);
    }

    wait(NULL);
    puts("End");

    exit(0);
}
