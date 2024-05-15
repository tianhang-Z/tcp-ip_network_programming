#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

int main(){
    int fd1,fd2,fd3;
    fd1=socket(PF_INET,SOCK_STREAM,0);
    fd2=open("data.txt",O_RDONLY);
    fd3=socket(PF_INET,SOCK_DGRAM,0);
    printf("file descriptors :%d,%d,%d \n",fd1,fd2,fd3);
    close(fd1);close(fd2);close(fd3);
    return 0;
}
