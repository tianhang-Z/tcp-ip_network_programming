#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char * argv[]){
    int status;
    pid_t pid=fork();

    if(pid==0) {
        sleep(15);
        return 24;
    }
    else {
        int p1;
        while((p1=waitpid(-1,&status,WNOHANG))==0){
            sleep(3);
            puts("sleep 3sec.");
        }

        if(WIFEXITED(status)){
            printf("child send: %d\n",WEXITSTATUS(status));
            printf(" child pid from waitpid :%d\n",p1);
        }
    }
    return 0;

}