#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <dirent.h>


#define PAT "/etc"


int main()
{
    DIR * dp;
    struct dirent * cur;

    dp = opendir(PAT);
    if(dp == NULL)
    {
        perror("dir open fail\n");
        exit(1);
    }

    while((cur = readdir(dp)) != NULL)
    {
        puts(cur->d_name);

    }

    closedir(dp);

}