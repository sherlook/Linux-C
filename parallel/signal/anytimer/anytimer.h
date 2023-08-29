 /*
  使用单一计时器，构造一组函数，实现任意数量的定时器
  计时器到时间时，执行函数并传参
 */
 #ifndef ANYTIMER_H__
 #define ANYTIMER_H__

// typedef void timer_t;
typedef void at_jobfunc_t(void *);
#define JOB_MAX 10000

int anytimer_addjob(int time, at_jobfunc_t * jobptr, void *arg);
/*
 * 增加一个定时任务
 * return:   >= 0       成功，返回任务ID
 *           == -EINVAL 失败，参数非法
 *           == -ENOSPC 失败，数组满
 *           == -ENOMEM 失败，内存空间不足
 */



int anytimer_canceljob(int id);
/*
 * 取消一个定时任务,当前任务永远不再继续了
 * return:   >= 0       成功，任务成功取消
 *           == -EINVAL 失败，参数非法
 *           == -EBUSY  失败，指定任务已完成
 *           == -ECANCELED 失败，指定任务重复取消
 */


int anytimer_waitjob(int id);
/*
 * 回收一个定时任务，需要先取消任务，再进行回收
 * return:   == 0       成功，指定任务成功释放
 *           == -EINVAL 失败，参数非法
 */


int anytimer_pausejob(int id);
/*
 * 暂停一个定时任务
 * return:   == 0       成功，指定任务成功释放
 *           == -EINVAL 失败，参数非法
 */


int anytimer_resumejob(int id);
/*
 * 重启一个定时任务
 * return:   == 0       成功，指定任务成功释放
 *           == -EINVAL 失败，参数非法
 */


#endif