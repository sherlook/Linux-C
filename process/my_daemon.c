/**
 * 守护进程
*/
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<syslog.h>
#include<string.h>
#include<errno.h>

#define FNAME "/tmp/out"

static int daemonize(void)
{
    //子进程调用setsid()
    int fd;
    pid_t pid;

    pid = fork();
    if(pid < 0)
        return -1;
    if(pid > 0)
        return 0;

    // 标准输入输出重定向
    fd = open("/dev/null", O_RDWR);
    if(fd < 0)
        return -1;


    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if(fd > 2)
        close(fd);

    setsid();

    // 守护进程一直在跑， chdir()
    chdir("/");
    // 不创建文件umask()
    //umask(0);

    return 0;
}

int main()
{   
    FILE * fp;
    int i;

    //syslog
    openlog("My_daemon", LOG_PID, LOG_DAEMON);

    // 守护进程
    if(daemonize())
    {   
        // 不能加入'\n' 格式是由syslog控制的
        syslog(LOG_ERR, "daemon failed");
    }
    else
    {
        syslog(LOG_INFO, "daemon sucessded");
    }


    //后台持续写入数字
    fp = fopen(FNAME, "w");
    if(!fp)
    {   
        //没有了控制终端，输出已经没用了
        syslog(LOG_ERR, "fopen():%s", strerror(errno));
        exit(1);
    }

    syslog(LOG_INFO, "%s was opened.", FNAME);
    
    for(i=0; ; i++)
    {
        fprintf(fp, "%d\n", i);
        //文件是全缓冲， 终端是行缓冲，刷新才能看到
        fflush(fp);
        sleep(1);
    }

    fclose(fp);
    closelog();
    exit(0);
}























