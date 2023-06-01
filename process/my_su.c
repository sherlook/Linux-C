/**
 * 实现linux下 su 命令   su  uid   ls -l   切换到相应用户执行相应命令
 * 1.需要切换到root用户 将my_su所属用户变为root  chown root my_su
 *                    且权限改为r+s           chmod u+s my_su
 * 
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char ** argv)
{   
    pid_t pid;
    if(argc<3)
    {
        fprintf(stderr, "Usage:%s + uid + cmd\n", argv[0]);
        exit(1);
    }

    pid = fork();
    if(pid<0)
    {
        perror("fork()\n");
        exit(1);
    }
    if(pid == 0)
    {   
        setuid(atoi(argv[1]));

        execvp(argv[2], argv+2);
        perror("exec()\n");
        exit(1);
    }

    wait(NULL);

    exit(0);
}