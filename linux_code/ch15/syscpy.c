#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define BUF_SIZE 3

int main(){
    int fd1,fd2;
    int len=0;
    char buf[BUF_SIZE];

    clock_t start,end;
    start = clock();
    //…calculating…
    
    printf("start\n-----");
    for(int i=1;i<1000;i++){
        fd1=open("news.txt",O_RDONLY);
        fd2=open("cpy.txt",O_WRONLY|O_CREAT|O_TRUNC);
        while((len=read(fd1,buf,sizeof(buf)))>0)
            write(fd2,buf,len);
    }
    end = clock();
    printf("time=%f\n",(double)(end-start)/CLOCKS_PER_SEC);
    printf("end-----\n");
    return 0;
}
