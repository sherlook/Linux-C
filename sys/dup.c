#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define FILENAME "123"
int main()
{
// 写入FILENAME文件，不修改**下面代码

    int fd;
    fd = open(FILENAME, O_WRONLY|O_CREAT|O_TRUNC, 0600);
    if(fd<0)
    {
        perror("open fial\n");
        exit(1);
    }   

    close(1);
    dup(fd);
    close(fd);

/********************/

    puts("a");  //->stdout
    exit(0);
}























