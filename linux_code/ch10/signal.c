#include <stdio.h>
#include <unistd.h>
#include <signal.h>



//sig的值取决于timeout函数在signal函数中和哪个信号关联
void timeout(int sig){
    if (sig==SIGALRM)
    {
        printf("timeout SIGALRM sig:%d\n",sig);
    }
    alarm(2);
}

void keycontrol(int sig){
    if(sig==SIGINT)
        printf("ctrl+c pressed SIGINT sig:%d\n",sig); 
}

int main(int argc,char* argv[]){
    int i;
    signal(SIGALRM,timeout);
    signal(SIGINT,keycontrol);

    alarm(2);
    for(i=0;i<3;i++){
        puts("wait...");
        sleep(100);
    }
    return 0;
}
