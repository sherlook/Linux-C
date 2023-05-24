/*
 * 实现du指令，打印出文件目录所占K数 
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
#include<glob.h>

#define PATHSIZE 1024


static int path_noloop(const char * path)
{
    // 判断path 是不是 . or .. 
    // /aaa/bbb/ccc/ddd/eee/fff/ggg/.

    //找到最后一个字符/
    char * pos = NULL;
    pos = strrchr(path, '/');
    if(pos == NULL)
        exit(1);
    if(strcmp(pos+1, ".")==0 || strcmp(pos+1, "..")==0)
    {
        return 0;
    }
    return 1;

}


static long long mydu(const char * path)
{
    static struct stat stbuf;
    static char nextpath[PATHSIZE];
    glob_t glob_res;
    int err;
    int i;
    long long sum;

    if(lstat(path, &stbuf) == -1)
    {
        perror("lstat fail\n");
        exit(1);
    }

    //判断是目录文件还是非目录文件
    if(!S_ISDIR(stbuf.st_mode))
    {
        return stbuf.st_blocks;
    }

    //目录文件  /etc/aaa/bbb/ccc/ddd/eee/ffff
        //目录下所有文件   /etc/aaa/bbb/ccc/ddd/eee/ffff/*
        //目录下所有隐藏文件   /etc/aaa/bbb/ccc/ddd/eee/ffff/.*
    strncpy(nextpath, path, PATHSIZE);
    strncat(nextpath, "/*", PATHSIZE);

    // printf("%s\n", nextpath);

    // 解析nextpath 非隐藏文件
    err = glob(nextpath, 0, NULL, &glob_res);
    if(err)
    {
        perror("glob1()\n");
        exit(1);
    }


    for(i=0; i<glob_res.gl_pathc; i++)
    {   
        printf("gl_pathv[%d]: %s\n",i,glob_res.gl_pathv[i]);
    }


    strncpy(nextpath, path, PATHSIZE);
    strncat(nextpath, "/.*", PATHSIZE);


    // 解析nextpath 隐藏文件
        // 解析当前隐藏文件时，会解析出. .. .表示当前目录，glob()解析.时，会回到当前目录
        // 递归只能处理树状结构， 当路径中存在回路时，无法使用
    err = glob(nextpath, GLOB_APPEND, NULL, &glob_res);
    if(err)
    {
        perror("glob2()\n");
        exit(1);
    }

    sum = 0;
    //当前路径文件大小
    sum += stbuf.st_blocks;

    for(i=0; i<glob_res.gl_pathc; i++)
    {   
        // 判断 不是. .. 才进行递归
        if(path_noloop(glob_res.gl_pathv[i]))
            sum += mydu(glob_res.gl_pathv[i]);
    }

    globfree(&glob_res);

    return sum;
}

int main(int argc, char ** argv)
{
    if(argc<1)
    {
        fprintf(stderr, "Usage: %s file or directory!\n", argv[1]);
        exit(1);
    }
    printf("%lld \n", mydu(argv[1])/2);
}