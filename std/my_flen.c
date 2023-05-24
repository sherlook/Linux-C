/**
 * 获取文件大小flen()
 * 
 * 1. fgetc()计算文件大小
 * 2. fseek() ftell()计算文件大小
 * 
*/
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** argv)
{
    FILE * fp=NULL; // FILE POINT SRC
    int ch;
    int cnt=0;

    // 用到命令行传参，判断参数个数
    if(argc < 2)
    {
        fprintf(stderr, "Usage:%s <src_file>\n", argv[0]);
        exit(1);
    }


    fp = fopen(argv[1], "r");
    if(fp == NULL)
    {
        perror("fopen() 0failed!");
        exit(1);
    }

    // 1. fgetc()获取文件大小
    while(fgetc(fp) != EOF)
    {
        cnt ++;
    }

    // 2.fseek() ftell()获取文件大小
    fseek(fp, 0L, SEEK_END);
    printf("%ld\n",ftell(fp));

    fclose(fp);
    printf("cnt :%d",cnt);

    exit(0);

}