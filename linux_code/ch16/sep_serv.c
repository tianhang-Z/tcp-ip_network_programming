#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024
void error_handling(char *message);
/*
服务端
支持五次连接和断开

测试 flose(writeup) 是否可以半关闭 
*/
int main(int argc,char *argv[]){

    int serv_sock,clnt_sock;
    char message[BUFF_SIZE]={0};
    int str_len,i;

    FILE *readfp;
    FILE *writefp;

    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc!=2){
        printf("usage :%s <port> \n",argv[0]);
        exit(1);
    }

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
    
    clnt_adr_sz=sizeof(clnt_adr);
    //支持五次连接
    for(int i=0;i<5;i++){
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        if(clnt_sock==-1) error_handling("accept() error");

        //套接字转换为FILE*
        readfp=fdopen(clnt_sock,"r");
        writefp=fdopen(clnt_sock,"w");

        //服务端发送后 fclose关闭writefp
        fputs("FROM SERVER : HI Client \n",writefp);
        fputs("love the world \n",writefp);
        fflush(writefp);
        fclose(writefp);

        //测试fclose之后能否正常接收
        //fclose 会关闭套接字 因此不能接收
        if(fgets(message,BUFF_SIZE,readfp)!=NULL){
            fputs(message,stdout);
            fflush(stdout);
        }
        else
            printf("------acpt NULL-------\n");
        fclose(readfp);

    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}