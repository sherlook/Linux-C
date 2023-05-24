#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{   
    FILE * fp;
    char * linebuffer = NULL;
    size_t linesize = 0;

    if(argc < 2 )
    {
        fprintf(stderr, "Usage: %s <filename>", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if(fp == NULL)
    {
        perror("openfile()");
        exit(1);
    }

    while(1)
    {
        if(getline(&linebuffer, &linesize, fp) < 0)
            break;
        printf("%d\n", strlen(linebuffer));
        printf("%d\n", linesize);
    }

    fclose(fp);
    exit(0);
}