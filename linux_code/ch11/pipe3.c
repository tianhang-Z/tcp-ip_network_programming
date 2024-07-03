#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(){
    int fds1[2],fds2[2];
    char str1[]="who are you?";
    char str2[]="thanks for your messgae!";
    char buf[BUF_SIZE];
    __pid_t pid;

    pipe(fds1);
    pipe(fds2);
    pid=fork();
    //fd[0] out fd[1] in
    if(pid==0){
        //child fds1[0] out     fds2[1] in
        write(fds2[1],str1,sizeof(str1));
        read(fds1[0],buf,BUF_SIZE);
        printf("child proc out:%s\n",buf);
    }
    else{
        //parent fds1[1] in     fds2[0] out
        write(fds1[1],str2,sizeof(str2));
        read(fds2[0],buf,BUF_SIZE);
        printf("parent proc out: %s\n",buf);
    }
    return 0;
}