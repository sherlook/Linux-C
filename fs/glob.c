/* glob()函数解析目录
 * 解析模式/通配符
 * 
*/


#include <stdio.h>
#include <stdlib.h>
#include <glob.h>


//
#define PAT "/etc/a*.conf"

// /etc下的所有非隐藏文件
#define PAT "/etc/*"

// /etc下的隐藏文件
#define PAT "/etc/.*"

int errfunc_ (const char * errpath, int eerrno)
{
    puts(errpath);
    fprintf(stderr, "Error Message: %s\n", strerror(eerrno));
    return 0;
}

int main()
{
    glob_t glob_res;
    int err, i;

    err = glob(PAT, 0, &errfunc_, &glob_res);
    if(err)
    {
        printf("Error Code:%d\n", err);
        exit(1);
    }
    
    for(i=0; i<glob_res.gl_pathc; i++)
    {
        puts(glob_res.gl_pathv[i]);
    }

    globfree(&glob_res);
    exit(0);
}