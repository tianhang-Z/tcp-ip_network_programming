#define _XOPEN_SOURCE          

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <assert.h>

#define BUF_SIZE 30
void error_handling(char *buf);
void urg_handler(int signo);

int acpt_sock;   //看门端口
int recv_sock;   //连接端口

int main(int argc,char* argv[]){

    if(argc!=2){
        printf("usage :%s <port> \n",argv[0]);
        exit(1);
    }

    struct sockaddr_in recv_adr,send_adr;
    socklen_t serv_adr_sz;
    char buf[BUF_SIZE]={0};

    //设置信号
    struct sigaction act;
    act.sa_handler=urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;

    //接收端
    acpt_sock=socket(PF_INET,SOCK_STREAM,0);

    memset(&recv_adr,0,sizeof(recv_adr));
    recv_adr.sin_family=AF_INET;
    recv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    recv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(acpt_sock,(struct sockaddr*)&recv_adr,sizeof(recv_adr))==-1)
        error_handling("bind() error ");

    if(listen(acpt_sock,5)==-1) error_handling("listen() error");

    socklen_t adr_sz=sizeof(send_adr);
    recv_sock=accept(acpt_sock,(struct sockaddr*)&send_adr,&adr_sz);

    fcntl(recv_sock,__F_SETOWN,getpid());
    int state=sigaction(SIGURG,&act,0);
    assert(state==0);

    int str_len=0;
    while((str_len=recv(recv_sock,buf,sizeof(buf),MSG_OOB))!=0){
        if(str_len==-1) continue;
        buf[str_len]=0;
        puts(buf);
    }
    close(recv_sock);
    close(acpt_sock);
    return 0;

}

void urg_handler(int signo){
    int str_len;
    char buf[BUF_SIZE];
    str_len=recv(recv_sock,buf,sizeof(buf)-1,MSG_OOB);
    buf[str_len]=0;
    printf("urgent message : %s \n",buf);
    return ;
}

void error_handling(char *buf){
    fputs(buf,stderr);
    fputc('\n',stderr);
    exit(1);
}
