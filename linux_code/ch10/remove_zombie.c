#define _XOPEN_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig){
    int status;
    if(sig==SIGCHLD){
        pid_t pid=waitpid(-1,&status,WNOHANG);
        if(WIFEXITED(status)){
            printf("child proc pid %d \n",pid);
            printf("child proc return %d \n",WEXITSTATUS(status));
        }   
    }
}

int main(){
    pid_t pid;
    struct sigaction act;
    act.sa_handler=read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    sigaction(SIGCHLD,&act,0);

    pid=fork();
    if(pid==0){
        puts("child proc one");
        sleep(5);
        return 10;
    }
    else{
        pid=fork();
        if(pid==0){
            puts("child proc two");
            sleep(10);
            exit(20);
        }
        else{

            for(int i=0;i<5;i++){
                puts("wait..");
                sleep(3);
            }
        }
    }

}