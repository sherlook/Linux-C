/**
 * 实现cp指令
 * fread()
 * fwrite()
*/

#include <stdlib.h>
#include <stdio.h>
#define BUFFERSIZE 1024

int main(int argc, char ** argv)
{
    FILE * fps=NULL; // FILE POINT SRC
    FILE * fpd=NULL;
    char buff[BUFFERSIZE];
    int n;

    // 用到命令行传参，判断参数个数
    if(argc < 3)
    {
        fprintf(stderr, "Usage:%s <src_file> <dest_file>\n", argv[0]);
        exit(1);
    }


    fps = fopen(argv[1], "r");
    if(fps == NULL)
    {
        perror("fopen() 0failed!");
        exit(1);
    }

    fpd = fopen(argv[2], "w");
    if(fpd == NULL)
    {
        perror("fopen() 1failed!");
        exit(1);
    }

    while((n = fread(buff, 1, BUFFERSIZE, fps)) > 0)
    {  
        fwrite(buff, 1, n, fpd);
    }

    fclose(fpd);
    fclose(fps);
    exit(0);

}