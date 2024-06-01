/*
1,socket     分配socket
2,bind         配置ip
3,listen      套接字设置为可接受连接
4,accept
# 基于linux 需要unistd.h 
# 基于Windows
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);
//字符串常量是个地址，可以给指针赋值

int main(int argc,char* argv[]){
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[]="hello world!";
    char message1[]="twice";

    if(argc!=2){ 
        printf("usage : %s <port>/n",argv[0]);
        exit(1);
    }
    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1) 
        error_handling("socket() error");

    memset(&serv_addr,0,sizeof(serv_addr));  //serv_addr 结构体清零
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*) &serv_addr,sizeof(serv_addr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");
    clnt_addr_size=sizeof(clnt_addr);
    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if(clnt_sock==-1)
        error_handling("accept() error");

    char ack[100]={0};
    write(clnt_sock,message,sizeof(message)-1);
    write(clnt_sock,message1,sizeof(message1));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}