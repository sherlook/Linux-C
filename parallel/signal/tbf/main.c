/**
 * 系统调用实现slow_cat,进行流量控制(每隔一段时间输出一定的字符)，音频视频中使用，网络 
 *  siganl设置alarm()的信号处理程序，程序中使用loop控制进行字符的读写    
    alarm()计时1s  单次alarm(1)只能计时一次，需要循环产生信号
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
#include <string.h>
#include  "mytbf.h"

#define CPS      10            //每秒钟传输速率 character per second
#define BUFFSIZE 1024
#define BURST    100           // Token上限

static volatile int token = 0;

int main(int argc , char ** argv)
{   
    int sfd, dfd;
    dfd = 1;
    char buf[BUFFSIZE];  
    int len,ret,pos;
    mytbf_t * mytbf_p;
    int size = 0;

    // 判断输入命令是否合法
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s <s_file> <d_file>\n", argv[0]);
        exit(1);
    }

    // 初始化令牌桶
    mytbf_p = mytbf_init(CPS, BURST);
    if(mytbf_p == NULL)
    {
        fprintf(stderr, "mytbf_init() fail!\n");
        exit(1);
    }

    // 打开需要读取的文件，不被信号打断
    // do中需要先执行一次
    do
    {
        sfd = open(argv[1], O_RDONLY);
        printf("sfd:%d\n", sfd);
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

    while(1)
    {   
        //从令牌桶中取出BUFFSIZE个token,获得size个token
        size = mytbf_fetchtoken(mytbf_p, BUFFSIZE);
        if(size < 0)
        {
            fprintf(stderr, "mytbf_fetchtoken():%s\n", strerror(-size));
            exit(1);
        }

        while((len = read(sfd, buf, size)) < 0)
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

        // 防止有token没用完，需要还回去
        if(size - len > 0)
            mytbf_returntoken(mytbf_p, size-len);

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
    mytbf_destory(mytbf_p);

    exit(0);
}