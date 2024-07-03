#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(){
    int fd[2];
    char str1[]="who are you?";
    char str2[]="thanks for your messgae!";
    char buf[BUF_SIZE];
    __pid_t pid;

    pipe(fd);
    pid=fork();
    //fd[0] out fd[1] in
    if(pid==0){
        //child
        write(fd[1],str1,sizeof(str1));
        sleep(2);
        read(fd[0],buf,BUF_SIZE);
        printf("child proc out:%s\n",buf);
    }
    else{
        //parent 
        read(fd[0],buf,BUF_SIZE);
        printf("parent proc out: %s\n",buf);
        write(fd[1],str2,sizeof(str2));
        sleep(3);
    }
    return 0;
}