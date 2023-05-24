#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char ** argv)
{   
    if(argc < 2)
    {
        fprintf(stderr, "Usage :%s <file_name>\n", argv[0]);
        exit(1);
    }
    int fd;
    fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0600);
    if(fd < 0)
    {
        perror("open fail \n");
        exit(1);
    }

    //没有单位的数值是没有意义的 5G>int
    lseek(fd, 5LL*1024LL*1024LL*1024LL-1LL, SEEK_SET);

    write(fd, "", 1);

    close(fd);

    exit(0);

}
