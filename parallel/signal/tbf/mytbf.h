#ifndef MYTBF_H__
#define MYTBF_H__


#define MYTBF_MAX   1024

// 对用户进行数据结构隐藏，不知道.c中的结构体是如何设计的，用void
typedef void mytbf_t;

// 初始化一个令牌桶，返回令牌桶的指针
mytbf_t * mytbf_init(int cps, int burst);

// 取令牌
int mytbf_fetchtoken(mytbf_t *, int);

// 返还令牌
int mytbf_returntoken(mytbf_t *, int);

// 销毁一个令牌桶
int mytbf_destory(mytbf_t * );
#endif