#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(){
    int fd[2];
    char str[]="who are you?";
    char buf[BUF_SIZE];
    __pid_t pid;

    pipe(fd);
    pid=fork();
    //fd[0] out fd[1] in
    if(pid==0){
        //child in
        write(fd[1],str,sizeof(str));
    }
    else{
        //parent out
        read(fd[0],buf,BUF_SIZE);
        puts(buf);
    }
    return 0;
}