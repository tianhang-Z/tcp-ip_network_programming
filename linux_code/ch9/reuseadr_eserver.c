#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define TRUE 1
#define FALSE 0
void error_handling(char *message);

int main(int argc,char *argv[]){
    int serv_sock,clnt_sock;
    char message[30];
    int str_len,option;

    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t clnt_adr_sz,optlen;

    if(argc!=2){
        printf("usage :%s <port> \n",argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1) 
        error_handling("socket() error\n");

    
    optlen=sizeof(option);
    option=TRUE;
    setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,(void *)&option,optlen);
    

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");

    //1次连接
    for(int i=0;i<1;i++){
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        clnt_adr_sz=sizeof(clnt_adr);
        if(clnt_sock==-1) error_handling("accept() error");

        while((str_len=read(clnt_sock,message,sizeof(message)))!=0){
            printf("str_len:%d\n",str_len);
            write(clnt_sock,message,str_len);
            write(1,message,str_len);
        }
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