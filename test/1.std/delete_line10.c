/**
 * 删除文件的第十行
 *  方法一：
 *      后一行覆盖前一行
 *  方法二：
 *      一个进程打开两次相同文件
 *  方法三：
 *      两个进程打开相同文件
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define MAXSIZE 100

int main()
{   
    FILE * fp;
    char * line = NULL;
    size_t len=0;
    size_t nread = 0;
    int cnt=0;
    fp = fopen("abc", "r+");
    if(!fp)
    {
        perror("file open fail\n");
        exit(1);
    }

    int i = 10;
    int j = 11;

    //获取文件总长度
    long file_len = 0;
    fseek(fp, 0L, SEEK_END);
    file_len = ftell(fp);
    //printf("文件总长度为： %ld\n", file_len);

    //删除那句长度
    long line10_len = 0L;

    fseek(fp, 0L, SEEK_SET);
    while(1)
    {
        if(getline(&line, &len, fp) < 0)
            break;        
        //文件行数
        cnt++;
        //删除那句长度
        if(cnt == i)
        {
            line10_len = strlen(line);
            printf("删除的句子长度为： %ld\n", line10_len);
        }
        // printf("string :%s", line);
        // printf("strlen(line) :%ld \n", strlen(line));
    }

    char tmp[MAXSIZE];
    tmp[0] = '\0';
    int cur_i = 1;
    int cur_j = 1;

    for(; j<=cnt; j++)
    {
        //读j行
        fseek(fp, 0, SEEK_SET);
        cur_j = 1;
        cur_i = 1;
        while(cur_j <= j)
        {   
            getline(&line, &len, fp);
            //printf("j=%d;  cur_j=%d;  str_line:%s;", j, cur_j, line);
            cur_j++;
        }
        //printf("j第 %d 行数据为： %s", j, line);
        tmp[0] = '\0';
        strncpy(tmp, line, MAXSIZE);
        //printf("tmp: %s",tmp);

        //写i行
        fseek(fp, 0, SEEK_SET);
        while(cur_i < i)
        { 
            getline(&line, &len, fp);
            cur_i++;
        }
        //printf("i第 %d 行数据为： %s", cur_i, line);
        //fprintf(fp, "%s\n", "a");
        fprintf(fp, "%s", tmp);
        i++;
    }

    //截断文件多余长度
    truncate("abc", (file_len-line10_len));

    fclose(fp);
    exit(0);
}
