/*
int socket(int domain, int type, int protocol);  
#domain 协议族 一般是 PF_INET 指IPv4
#type 套接字数据传输方式 
*/
/*
1,socket
2,connect
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   //提供os访问接口
#include <arpa/inet.h>   //处理ip
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc,char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[100]={0};
    int str_len=0;
    int idx=0,read_len=0;

    if(argc!=3){
        printf("usage: %s <IP> <port> \n",argv[0]);
        exit(1);
    }

    sock=socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1)
        error_handling("socket() error");
    
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("connect() error");
    
    //一次write 多次read
    // while (read_len=read(sock,&message[idx++],1))
    // {
    //     if(read_len==-1)
    //         error_handling("read() error");
        
    //     str_len+=read_len;
    // }
    // printf("call read() count:%d \n ",str_len);
    
    
    //多次write 一次read
    printf("wait i:3000 \n");
    for(int i=0;i<3000;i++){

    }

    str_len=read(sock,message,sizeof(message));
    if(str_len==-1)
        error_handling("read() error");
    
    printf("message from server :%s \n" ,message);
    printf("message size count:%d \n ",str_len);
    close(sock);
    return 0;
}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}