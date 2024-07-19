#define _XOPEN_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//IO分离的客户端
//write_routine进程 shutdown半关闭
#define BUFF_SIZE 30
void error_handling(char *message);
void read_routine(int sock,char* buf);
void write_routine(int sock,char* buf);

int main(int argc,char *argv[]){

    int sock;
    char message[BUFF_SIZE]={0};

    struct sockaddr_in serv_adr;

    if(argc!=3){
        printf("usage :%s <ip> <port> \n",argv[0]);
        exit(1);
    }

    sock=socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1) error_handling("socket() error\n");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));
    
    if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("connect() error");

    pid_t pid;
    pid=fork();
    //读写分离
    if(pid==0) write_routine(sock,message);
    else read_routine(sock,message);
 
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
void read_routine(int sock,char* buf){
    while(1){
        int str_len=read(sock,buf,BUFF_SIZE);
        if(str_len==0){
            return ;
        }
        buf[str_len]=0;
        printf("message from serv:%s\n",buf);
    }
}
void write_routine(int sock,char* buf){
    while(1){
        puts("input message ,press Q to end");
        fgets(buf,BUFF_SIZE,stdin);
        if(strcmp(buf,"Q\n")==0||strcmp(buf,"q\n")==0){
            shutdown(sock,SHUT_WR);
            return ;
        }
        write(sock,buf,strlen(buf));
    }
}