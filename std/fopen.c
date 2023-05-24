#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(){
    FILE * fp = NULL;
    fp = fopen("123","r+");
    if(fp == NULL){
        // fprintf(stderr, "fopen() failed error=%d\n", errno);
        // perror("fopen()");
        fprintf(stderr, "fopen() failed error=%s\n", strerror(errno));
        exit(1);
    }

    fclose(fp);
    exit(0);
}