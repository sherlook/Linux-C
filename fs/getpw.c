#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pwd.h>

int main(int argc, char ** argv)
{
    if(argc < 1)
    {
        fprintf(stderr, "Usage: %s userid\n", argv[0]);
        exit(1);
    }

    struct passwd * getpwline;

    getpwline = getpwuid(atoi(argv[1]));

    printf("user name: %s\n", getpwline->pw_name);
    printf("user passwd: %s\n", getpwline->pw_passwd);

    exit(0);
}
