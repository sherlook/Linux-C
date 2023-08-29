#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "mytbf.h"

typedef struct mytbf_st
{
    int cps;
    int token;
    int burst;
    int pos;
}mytbf_st;

typedef void (*sighandler_t)(int);

static mytbf_st * mytbf_list[MYTBF_MAX];
static int inited = 0;
static sighandler_t alrm_handler_save;



static int get_free_pos()
{
    int i=0;
    for(i; i<MYTBF_MAX; i++)
    {
        if(mytbf_list[i] == NULL)
            return i;
    }
    return -1;
}

static int min(int a, int b)
{
    if(a < b)
        return a;
    else 
        return b;
}

static void alrm_handler()
{
    alarm(1);
    // 到时间之后
    for(int i=0; i<MYTBF_MAX; i++)
    {
        //当前令牌有任务存在
        if(mytbf_list[i] != NULL)
        {
            mytbf_list[i]->token += mytbf_list[i]->cps;
            if(mytbf_list[i]->cps > mytbf_list[i]->burst)
                mytbf_list[i]->cps = mytbf_list[i]->burst;
        }
    }
}

static void module_unload()
{
    // 对功能进行恢复，关掉信号
    signal(SIGALRM,alrm_handler_save);
    alarm(0);
    for(int i=0; i<MYTBF_MAX; i++)
        free(mytbf_list[i]);
}

static void module_load()
{
    // 保存之前的alrm信号的处理函数
    alrm_handler_save = signal(SIGALRM, alrm_handler);
    alarm(1);

    // 钩子函数，程序正常终止的时候进行调用
    atexit(module_unload);
}

// 初始化一个令牌桶，返回令牌桶的指针
mytbf_t * mytbf_init(int cps, int burst)
{
    mytbf_st * me;
    
    // 只有第一次会调用，之后再也用不到了
    if(!inited)
    {   
        module_load();
        inited = 1;
    }

    int pos;
    pos = get_free_pos();
    if(pos < 0)
        return NULL;

    me = malloc(sizeof(*me));
    if(me == NULL)
        return NULL;

    me->cps = cps;
    me->token = 0;
    me->burst = burst;
    me->pos = pos;

    mytbf_list[pos] = me;

    return me;
}

// 取令牌
int mytbf_fetchtoken(mytbf_t * ptr, int size)
{   
    // 非法操作
    if(size <= 0 )
        return -EINVAL;

    mytbf_st * me = ptr;

    // 没有token，则等待 
    while(me->token <= 0)
        pause();

    int n;
    n = min(me->token, size);

    // token >= size; 可以给,给size
    // token < size;  可以给,给token
    me->token -= n;
    return n;
}

// 返还令牌
int mytbf_returntoken(mytbf_t *ptr, int size)
{
    // 非法操作
    if(size <= 0 )
        return -EINVAL;

    mytbf_st * me = ptr;

    me->token += size;
    if(me->token > me->burst)
        me->token = me->burst;
    
    return size;
}

// 销毁一个令牌桶
int mytbf_destory(mytbf_t * ptr)
{
    // mytbf是void类型的，void * 不能够取成员，需要先格式转换
    mytbf_st * me = ptr;
    mytbf_list[me->pos] = NULL;
    free(me);
    return 0;
}




