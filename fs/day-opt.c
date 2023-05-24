/**
 * 支持命令行传参的日期显示
 * -Y ：年份
 * -M ：月份
 * -D ：day
 * -h : hour
 * -m : minute
 * -s : second
 * 
 * 用getopt()实现
 *  获取选项和选项的传参
 *  选项： -y
 *  选项的传参： -y 4           ：实现
 *  非选项的传参：  /tmp/out    - 实现
 *      多个非选项传参时，可能需要保存起来
 *  
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<string.h>

#define BUFFSIZE 1024
#define FSIZE 1024

int main(int argc, char ** argv) 
{

    time_t tm;
    struct tm * ptm;
    char timebuf[BUFFSIZE];
    int ret=0;
    char formate[FSIZE];
    formate[0] = '\0';
    // puts(formate);
    // exit(0);
    time(&tm);
    ptm = localtime(&tm);
    FILE * fp = stdout;

    while(1)
    {
        ret = getopt(argc, argv, "-Y:MDh:ms");
        //printf("%d\n", ret);
        if(ret<0)
        {
            break;
        }

        switch(ret)
        {   
            case 1:
                // 非选项传参返回 1 
                fp = fopen(argv[optind-1], "w");
                if(!fp)
                {
                    fprintf(stderr, "file: %s open fail\n", argv[optind-1]);
                    // 文件打开失败  写stdout
                    fp = stdout;
                }
                break;
            case 'Y':
                if(strcmp(optarg,"2") == 0)
                {
                    //2位年份
                    strncat(formate, "%y ", FSIZE);
                }
                else if(strcmp(optarg,"4") == 0)
                {
                    //4位年份
                    strncat(formate, "%Y ", FSIZE);                        
                }
                else
                {
                    fprintf(stderr, "Invalid argument of -Y \n");
                    exit(1);
                }
                break;
            case 'M':
                strncat(formate, "%m ", FSIZE);
                break;
            case 'D':
                strncat(formate, "%d ", FSIZE);
                break;
            case 'h':
                strncat(formate, "%H ", FSIZE);
                break;
            case 'm':
                strncat(formate, "%M ", FSIZE);
                break;
            case 's':
                strncat(formate, "%S ", FSIZE);
                break;
            default:
                break;
        }
    }
    
    strftime(timebuf, BUFFSIZE, formate, ptm);
    fputs(timebuf, fp);
    if(fp != stdout)
        fclose(fp);
        


    exit(0);
}























