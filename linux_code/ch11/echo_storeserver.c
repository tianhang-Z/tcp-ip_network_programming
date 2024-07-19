#define _XOPEN_SOURCE 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

#define BUFF_SIZE 30
/*
实现了 1，保存收到的信息（利用子进程保存）
    2，基于多进程的回声服务端
*/
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc,char *argv[]){

    if(argc!=2){
        printf("usage :%s <port> \n",argv[0]);
        exit(1);
    }

    //sock相关变量
    int serv_sock,clnt_sock;
    char message[BUFF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t clnt_adr_sz;

    //signal相关变量
    int state;
    struct sigaction act;
    act.sa_handler=read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    state=sigaction(SIGCHLD,&act,0);

    //服务端配置socket bind listen accept
    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1) error_handling("socket() error\n");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");

    pid_t pid;
    int fd[2];
    pipe(fd);
    pid=fork(); //用于记录消息的子进程
    if(pid==0){
        FILE *fp=fopen("message.txt","wt");
        char msgbuf[BUFF_SIZE];
        int len;
        for(int i=0;i<10;i++){
            len=read(fd[0],msgbuf,BUFF_SIZE);
            fwrite((void *)msgbuf,1,len,fp);
        }
        fclose(fp);
        return 0;
    }
    
    while (1)
    {
        clnt_adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        if(clnt_sock==-1) 
            continue;
        else
            puts("new client connected...");
        
        pid=fork();
        if(pid==-1){
            //fork error   
            close(clnt_sock);
            continue;
        }
        if(pid==0){
            //子进程   回声 并且和保存消息的子进程进行通信
            close(serv_sock);
            while((str_len=read(clnt_sock,message,BUFF_SIZE))!=0){
                write(clnt_sock,message,str_len);
                write(fd[1],message,str_len);
            }
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }
        else
            close(clnt_sock);
    }
    
    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
void read_childproc(int sig){
    pid_t pid;
    int status;
    pid=waitpid(-1,&status,WNOHANG);
    printf("remove child proc id: %d\n",pid);
}
