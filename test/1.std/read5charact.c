/**
 * abc文件中存放5个字符  abcde
 * 用fgets()每次读取5个字符
 * 读取几次？ 
 * fgets每次读取 size-1个字符 
*/
#include<stdio.h>
#include<stdlib.h>

#define MAXSIZE 5

int main()
{
    
    FILE * fp = fopen("abc.txt", "r");
    if(fp == NULL)
    {
        fprintf(stderr, "file open fail\n");
        exit(1);
    }

    char str[MAXSIZE];
    int cnt=0;
    while(fgets(str, 5, fp))
    {
        cnt++;
    }
    puts(str);
    
    printf("read %d times\n", cnt);

    fclose(fp);
    exit(0);
}

