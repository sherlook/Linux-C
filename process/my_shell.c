/**
 * 模拟shell
 * 
 * 
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<glob.h>
#include<sys/wait.h>

#define DELIMS " \t\n"

struct cmd_st
{
    glob_t globres;
};

static void prompt(void)
{
    printf("myshell-0.1$");
}

static void parse(char * line, struct cmd_st * cmd)
{
    /*
       char *strtok(char *restrict str, const char *restrict delim);
       char *strsep(char **restrict stringp, const char *restrict delim);
       提取分隔后的每段字符串
        多个分割符可能解析出空串  ls /etc    /tmp
    */

    char * token;
    /*glob_t globres;  希望返回一个glob_t，要token出错则没法返回，则可以将glob_t放在参数中*/
    int i = 0;

    while(1)
    {
        token = strsep(&line, DELIMS);
        if(!token)
            break;
        if(token[0] == '\0')
            continue;
        /*
            如果存放在数组中，定长存储，无法确定size
            存放在类似agrv[]的数据结构中  ->   glob_t
            int glob(const char *restrict pattern, int flags,
                    int (*errfunc)(const char *epath, int eerrno),
                    glob_t *restrict pglob);
            GLOB_NOCHECK：If no pattern matches, return the original pattern
            GLOB_APPEND：追加 ，第一次不用追加
        */
       
        glob(token, GLOB_NOCHECK | GLOB_APPEND*i , NULL, &cmd->globres);
        i = 1;
    }


}

int main()
{   
    char * linebuf = NULL;
    size_t linebuf_size = 0;
    pid_t pid;
    glob_t * globres;
    struct cmd_st cmd;

    while(1)
    {
        //打印提示符
        prompt();
        //获取命令
        /**      ssize_t getline(char **restrict lineptr, size_t *restrict n,
                       FILE *restrict stream);
        */
        getline(&linebuf, &linebuf_size, stdin);

        //处理命令
        parse(linebuf, &cmd);

        if(0)
        {
            // 如果是内部命令
                 
        } 
        else
        {
            // 外部命令
            pid = fork();
            if(pid < 0)
            {
                perror("fork()");
                exit(1);
            }
            if(pid == 0)
            {
                /*int execvp(const char *file, char *const argv[]);*/
                execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
                perror("exec()");
                exit(1);
            }
            else
            {
                wait(NULL);
            } 
        }
    }
    exit(0);
}



































