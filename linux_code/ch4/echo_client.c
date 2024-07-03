#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*
回声客户端 存在缺陷（主要是因为tcp没有数据边界） 
问题1：数据太长 服务端分两次发送 客户端还未收到所有数据却从缓存区read
问题2：客户端多次write 服务端可能一次接受并返回
ch5解决该问题
*/
#define BUFF_SIZE 1024
void error_handling(char *message);

int main(int argc,char *argv[]){

    int sock;
    char message[BUFF_SIZE]={0};
    int str_len;

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

    while(1){
        fputs("input message (Q to quit)\n",stdout);
        fgets(message,BUFF_SIZE,stdin);

        if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
            break;

        write(sock,message,strlen(message));
        str_len=read(sock,message,BUFF_SIZE-1);
        printf("str_len: %d\n",str_len);
        //message[str_len]=0;
        printf("message from server: %s\n",message);

    }
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}