#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc,char *argv[]){
    int sock;
    int snd_buf=1024*3,rcv_buf=1024*3,state;
    socklen_t len;

    //set
    sock=socket(PF_INET,SOCK_STREAM,0);
    state=setsockopt(sock,SOL_SOCKET,SO_SNDBUF,(void *)&snd_buf,sizeof(snd_buf));
    if(state) error_handling("getsockopt error");

    state=setsockopt(sock,SOL_SOCKET,SO_SNDBUF,(void *)&rcv_buf,sizeof(rcv_buf));
    if(state) error_handling("getsockopt error");

    //get
    len=sizeof(snd_buf);
    state=getsockopt(sock,SOL_SOCKET,SO_SNDBUF,(void *)&snd_buf,&len);
    if(state) error_handling("getsockopt error");
    len=sizeof(rcv_buf);
    state=getsockopt(sock,SOL_SOCKET,SO_SNDBUF,(void *)&rcv_buf,&len);
    if(state) error_handling("getsockopt error");

    printf(" rcv buf size: %d, \n",rcv_buf);
    printf(" snd buf size: %d, \n",snd_buf);
    return 0;

}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}