/**
 * 守护进程，使用sigaction()解决守护进程被异常杀死之后，资源没有释放的问题
*/
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<syslog.h>
#include<string.h>
#include<errno.h>
#include<signal.h>

#define FNAME "/tmp/out"

static FILE * fp;

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

static void daemon_exit(int s)
{
    fclose(fp);
    closelog();
    exit(0);
}

int main()
{   
    int i;
    struct sigaction sa;

    sa.sa_handler = daemon_exit;

    // 响应SIGINT的时候 需要block SIGQUIT与SIGTERM，将这俩加入到set中
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGQUIT);
    sigaddset(&sa.sa_mask, SIGTERM);
    sigaddset(&sa.sa_mask, SIGINT); //BLOCK自己，因为在执行信号处理函数的时候会将当前信号的mask置位0
    
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    
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


}