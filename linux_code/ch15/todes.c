#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){
    FILE *fp;
    int fd=open("data1.txt",O_WRONLY|O_CREAT|O_TRUNC);
    if(fd==-1){
        fputs("error",stdout);
        exit(1);
    }

    printf("fisrt file des :%d\n",fd);
    fp=fdopen(fd,"w");
    fputs("TCP/IP socket\n",fp);
    printf("second file des: %d\n",fileno(fp));
    fclose(fp);
    return 0;
}