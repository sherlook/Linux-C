/**
 * 系统调用实现slow_cat,进行流量控制(每隔一段时间输出一定的字符)，音频视频中使用，网络 
 *  siganl设置setitimer()的信号处理程序
 *  漏桶算法与令牌桶算法：
        漏桶：
            流入速率：以任意速率流入
            流出速率：固定速率流出
        令牌桶（Token）:
            流入速率：固定速率往桶中存入token
            流出速率：流出时先从桶中取出token
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

#define CPS      10            //每秒钟传输速率 character per second
#define BUFFSIZE CPS

static volatile int loop = 1;

static void alrm_handler(int s)
{   
    loop = 0;
}


int main(int argc , char ** argv)
{   
    int sfd, dfd;
    dfd = 1;
    char buf[BUFFSIZE]; 
    int len,ret,pos;
    struct itimerval itv;

    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s <s_file> <d_file>\n", argv[0]);
        exit(1);
    }

    // do中需要先执行一次
    do
    {
        sfd = open(argv[1], O_RDONLY);
        if(sfd < 0)
        {   
            // 信号会打断阻塞的系统调用（open）
            // 判断出错原因是因为系统调用还是因为信号
            if(errno != EINTR)
            {       
                perror("file open fail\n");
                exit(1);
            }
        }

    }
    while(sfd < 0);

    // 注册SIGALRM信号的处理函数
    signal(SIGALRM, alrm_handler);

    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;

    if(setitimer(ITIMER_REAL, &itv, NULL) < 0)
    {
        perror("setitimer()");
        exit(1);
    }

    while(1)
    {
        // wait for signal if() or while()?
        // 实现一个阻塞，到了一秒钟的时候，程序才向下走
        while(loop)
        {
            pause();
        }
        loop = 1;

        // 循环从sfd读取
        while((len = read(sfd, buf, BUFFSIZE)) < 0)
        {
            // 报错，查看出错原因是不是信号产生的，       
            if(errno == EINTR)
                continue;
            perror("read() fail\n");
            break;
        }

        if(len == 0)
        {
            // zero indicates end of file
            break;
        }
        
        // 循环写入到dfd中
        pos = 0;
        while(len > 0)
        {
            // 读到了len个字节
            ret = write(dfd, buf+pos, len);
            if(ret < 0)
            {
                if(errno == EINTR)
                    continue;
                perror("write() fail\n");
                exit(1);
            }
            pos += ret;
            len -= ret;
        }
        
    }

    close(sfd);

    exit(0);
}