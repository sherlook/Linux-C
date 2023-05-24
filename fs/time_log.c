/**
 * time_log()
 * 实现功能：
 * 1. 输入到指定文件
 * 2. 输入格式为
 *             1 2023-5-16 18:58:20
 *             2 2023-5-16 18:58:21  
 *             3 2023-5-16 18:58:22
 *             程序结束  停止写入
 *             程序执行  继续写入
 *             4 2023-5-16 19:03:22
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

#define bufsize 1024


int main(int argc, char ** argv)
{
    // 1. 判断是否输入正确
    if(argc<1)
    {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(1);
    }

    // 2. 打开文件
    FILE * fd;
    fd = fopen(argv[1], "a+");
    if(!fd)
    {
        perror("open fail\n");
        exit(1);
    }

    // 3. 读行数
        // fgets()读到结尾，就可以知道行号了
    int line=0;
    char buf[bufsize];
    char * ret = NULL;
    while(1)
    {
        ret = fgets(buf, bufsize, fd);
        if(ret)
        {
            line++;
        }
        else
        {
            break;
        }
    }

    // 4. 写入文件
    time_t tm;    
    struct tm * ptm;

    while(1)
    {
        time(&tm);

        ptm = localtime(&tm);
        // 4 2023-5-16 19:03:22
        fprintf(fd, "%-4d %d-%d-%d %d:%d:%d\n", ++line,\
                ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday,\
                ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
        //除了写入终端设备，其他写入默认全缓冲模式，缓冲区写满了，才写入文件
        //所以查看文件看不到信息， 需要手动刷新缓冲区
        fflush(fd);
        sleep(1);
    }


    // 4. 关闭文件
    fclose(fd);
    exit(0);
}






















