/**
 * 实现cp指令
 * fgets()
 * fputs()
*/

#include <stdlib.h>
#include <stdio.h>
#define BUFFERSIZE 1024

int main(int argc, char ** argv)
{
    FILE * fps=NULL; // FILE POINT SRC
    FILE * fpd=NULL;
    char buff[BUFFERSIZE];

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

    while(fgets(buff, BUFFERSIZE, fps))
    {  
        fputs(buff, fpd); 
    }

    fclose(fpd);
    fclose(fps);
    exit(0);

}