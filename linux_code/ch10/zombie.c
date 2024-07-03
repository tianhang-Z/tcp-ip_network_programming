#include <stdio.h>
#include <unistd.h>

int main(int argc,char* argv[]){
    __pid_t pid=fork();
    if(pid==0){
        puts("child proc");
    }
    else{
        printf("child proc pid: %d \n",pid);
        sleep(30);
    }

    if(pid==0) puts("end child proc");
    else puts("end parent proc");
    return 0;
}