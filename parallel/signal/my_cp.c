/**
 * 系统调用实现cp指令
 * 防止信号打断阻塞系统调用
 * 
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFSIZE 1024


int main(int argc , char ** argv)
{   
    int sfd, dfd;
    char buf[BUFFSIZE];
    int len,ret,pos;
    if(argc < 3)
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
            // 判断出错原因是因为系统调用还是因为信号
            if(errno != EINTR)
            {       
                perror("file open fail\n");
                exit(1);
            }
        }

    }
    while(sfd < 0);


    do
    {
        dfd = open(argv[2], O_WRONLY| O_CREAT|O_TRUNC, 0600);
        if(dfd < 0)
        {
            close(sfd);
            perror("file open fail\n");
            exit(1);
        }
    }
    while(dfd < 0);




    while(1)
    {
        len = read(sfd, buf, BUFFSIZE);
        if(len < 0)
        {
            if(errno == EINTR)
                continue;
            perror("read() fail\n");
            break;
        }
        if(len == 0)
        {
            break;
        }
        pos = 0;
        while(len > 0)
        {
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

    close(dfd);
    close(sfd);

    exit(0);
}