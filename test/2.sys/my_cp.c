/**
 * 系统调用实现cp指令
 * 
*/
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

#define MAXSIZE 100

int main(int argc, char ** argv)
{
    if(argc<3)
    {
        fprintf(stderr, "Usage: %s <SOURCE> <DESTINY>\n", argv[0]);
        exit(1);
    }

    int sfd = 0;
    int dfd = 0;
    char buf[MAXSIZE];
    buf[0] = '\0';
    int ret = 0;
    int len = 0;
    int pos = 0;

    sfd = open(argv[1], O_RDONLY);
    if(sfd < 0)
    {
        perror("Source_file open fail\n");
        exit(1);
    }

    dfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if(dfd < 0)
    {
        perror("Destiny_file open fail\n");
        exit(1);
    }

    while(1)
    {       
        // 读数据
        ret = read(sfd, buf, MAXSIZE);
        if(ret < 0)
        {
            perror("read file error\n");
            exit(1);
        }
        if(ret == 0)
        {
            break;
        }

        // 写入文件
        pos = 0;
        while(ret > 0)
        {
            len = write(dfd, buf+pos, ret);
            if(len<0)
            {
                perror("write file error\n");
                exit(1);
            }
            ret -= len;
            pos += len;
        }
        
    }

    close(sfd);
    close(dfd);
    exit(0);
}
