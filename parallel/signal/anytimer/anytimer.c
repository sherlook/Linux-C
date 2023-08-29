#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "anytimer.h"

typedef int at_jobfunc(char *);
typedef void (*sighandler_t)(int);
typedef struct job
{   
    // 在数组中的下标任务ID
    int id;
    // 当前任务的状态：running:1 pause:0 over:-1 cancelled:-2
    int flag;
    // 需要定时多少sec
    int sec;
    // 到时间执行的函数
    at_jobfunc * func_p;
    // 执行函数的参数
    char * args;
}job;

static job* job_list[JOB_MAX];
static sighandler_t alrm_handler_save;
static int inited = 0;

static void alrm_handler()
{
    alarm(1);
    for(int i=0; i< JOB_MAX; i++)
    {
        // 当前为运行态的job
        if(job_list[i]!=NULL && job_list[i] -> flag == 1)
        {
            job_list[i] -> sec -= 1;
            // 进程到时间了:执行函数，修改状态
            if(job_list[i] -> sec == 0)
            {
                job_list[i] -> func_p(job_list[i] -> args);
                job_list[i] -> flag == -1;
            }
        }
    }
}

static void module_unload()
{
    // 对功能进行恢复，关掉信号
    signal(SIGALRM,alrm_handler_save);
    alarm(0);
    
    for(int i=0; i<JOB_MAX; i++)
        free(job_list[i]);
}

static void module_load()
{
    // 报错之前的SIGalrm信号处理函数
    alrm_handler_save = signal(SIGALRM, alrm_handler);
    
    // 开启定时
    alarm(1);
    
    // 钩子函数，程序正常终止的时候进行调用
    atexit(module_unload);

    printf("module_loead()\n");
}

int get_pos()
{
    int i=0;
    for(i; i<JOB_MAX; i++)
        if(!job_list[i])
            return i;
    return -1;
}


int anytimer_addjob(int time, at_jobfunc_t * jobptr, void *arg)
{
    // 参数非法
    if(time < 0 || arg == NULL)
    {   
        return EINVAL;
    }
    
    // 只有第一次会调用，之后再也用不到了
    if(!inited)
    {   
        module_load();
        inited = 1;
    }

    job * me = malloc(sizeof(job));
    if(!me)
        return ENOMEM;

    int id =get_pos();
    if(id < 0)
        return ENOSPC;

    job_list[id] = me;
    me->id = id;
    me->flag = 1;
    me->sec = time;
    me->func_p = jobptr;
    me->args = arg;

    return id;
}

int anytimer_canceljob(int id)
{
    if(id < 0)
        return EINVAL;
    
    if(job_list[id] ->flag == 1 || job_list[id] ->flag == 0)
    {
        // 当前任务在运行 or 暂停，取消任务
        job_list[id] ->flag = 0;
        return 0;
    }
    else if(job_list[id] -> flag == -2)
    {
        // 失败，指定任务重复取消
        return ECANCELED;
    }
    else
    {
        // 失败，指定任务已完成
        return EBUSY;
    }
}


int anytimer_waitjob(int id)
{
    // 参数非法，报错
    if(id < 0)
        return EINVAL;

    job* me = job_list[id];
    
    // 当前任务未取消，无法回收
    if(me->flag != -2)
        return EINVAL;

    free(me);
    job_list[id] = NULL;
    
    return id;
}

int anytimer_pausejob(int id)
{
    // 参数非法，报错
    if(id < 0)
        return EINVAL;

    job* me = job_list[id];
    me->flag = 0;

    return id;
}


int anytimer_resumejob(int id)
{
    // 参数非法，报错
    if(id < 0)
        return EINVAL;

    job* me = job_list[id];
    me->flag = 1;

    return id;
}