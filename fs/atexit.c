/**
 * atexit()注册
 *      在文件正常终止之后，以注册的逆序顺序进行调用注册函数
 *      puts()自带换行
*/

#include<stdio.h>
#include<stdlib.h>

static void func1(void)
{
    puts("func1");
}
static void func2(void)
{
    puts("func2");
}
static void func3(void)
{
    puts("func3");
}

int main()
{

    puts("begin");

    atexit(func1);
    atexit(func2);
    atexit(func3);

    puts("end");

    exit(0);
}















