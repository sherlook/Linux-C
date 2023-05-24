#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>

static __off_t flen(const char * filename)
{
    struct stat file_state;
    if(stat(filename, &file_state) < 0)
    {
        perror("file stat error\n");
        exit(1);
    }

    return file_state.st_size;
}

int main(int argc, char ** argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    printf("file length: %ld\n", flen(argv[1]));
    exit(0);

}

