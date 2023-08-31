/*
    sigaction()判断信号的来源，防止来自其他信号的攻击
    当前信号只相应来自kernel态的SIGALRM信号
    使用 while true; do kill -ALMR pid; done
    对当前进程发送ALRM信号也会打断流控
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

#define CPS      10            //每秒钟传输速率 character per second
#define BUFFSIZE CPS
#define BURST    100           // Token上限

static volatile sig_atomic_t token = 0;
static struct sigaction od_sigact;


static void alrm_handler(int s, siginfo_t *infop, void * unuse)
{   
    // 判断信号的来源，响应kernel，不响应其他的
    if(infop->si_code != SI_KERNEL)
        return;
    
    alarm(1);
    token ++;       // 攒一个token
    if(token > BURST)
        token = BURST;
}


int main(int argc , char ** argv)
{   
    int sfd, dfd;
    dfd = 1;
    char buf[BUFFSIZE];  
    int len,ret,pos;
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

    struct sigaction sa;
    sa.sa_sigaction = alrm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    
    sigaction(SIGALRM, &sa, &od_sigact);

    // signal(SIGALRM, alrm_handler);
    alarm(1);

    while(1)
    {
        // 没有token的时候等着
        while(token <= 0)
            pause();
        
        // 消耗掉一个token
        token--;

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


    sigaction(SIGALRM, &od_sigact, NULL);
    close(sfd);
    exit(0);
}