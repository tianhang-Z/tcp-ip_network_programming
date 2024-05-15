#include <stdio.h>
#include <stdlib.h>  //提供了size_t等类型 NULL等常量 free malloc exit等函数
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 100

void error_handling(char *message);

int main(){
    int fd;
    fd=open("data.txt",O_RDONLY);
    if(fd==-1)
        error_handling("open() error");
    else
        printf("file open,descriptor :%d \n",fd);
    char buf[BUF_SIZE];
    if(read(fd,buf,sizeof(buf))==-1)
        error_handling("read() error");
    else 
        printf("file data :%s \n",buf);
    close(fd);
    return 0;
}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}