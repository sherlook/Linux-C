/**
 * 系统io实现cp指令
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 1024 // 缓冲区大小

int main(int argc, char **argv) {
    // 源文件和目标文件的文件描述符
    int sfd, dfd;
    // 读写缓冲
    char buf[BUFSIZE];
    // len：读文件的返回字节数
    // ret：写文件的返回字节数
    // pos：写文件的当前位置
    int len, ret, pos;

    if(argc < 3) {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }
	
    // 以只读方式打开文件，打开文件失败
    if((sfd = open(argv[1], O_RDONLY)) < 0) {
        perror("open()");
        exit(1);
    }
	
    // 以只读方式打开文件，有则清空，无则创建
    // 打开文件失败
    if((dfd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0600)) < 0) {
        close(sfd);
        perror("open()");
        exit(1);
    }

    while(1) {
        if((len = read(sfd, buf, BUFSIZE)) < 0) {
            perror("read()");
            break;
        }
		
        // 读完文件
        if(len == 0)
            break;

        pos = 0;
        // 防止读到的字节没有完全写入文件
        // 保证读多少，就写多少
        while(len > 0) {
            if((ret = write(dfd, buf + pos, len)) < 0) {
                perror("write()");
                exit(1);
            }
            pos += ret;
            len -= ret;
        }
    }
	// 关闭文件描述符
    close(dfd);
    close(sfd);

    exit(0);
}
