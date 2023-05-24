/**
 * 文件位置指针
 * fseek()
 * ftell()
 * frewind()
*/

#include<stdio.h>
#include<stdlib.h>

int main()
{
    FILE * fp;
    fp = fopen("abc.txt","r");
    if(!fp)
    {
        perror("file open fail\n");
        exit(1);
    }


    fseek(fp, 0, SEEK_END);

    long pos;
    pos = ftell(fp);
    printf("pos : %ld\n", pos);

    fclose(fp);
    exit(0);
}