#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *buf);

int main(int argc,char *argv[]){
    int acpt_sock,recv_sock;
    struct sockaddr_in acpt_adr,recv_adr;
    char buf[BUF_SIZE];

    if(argc!=2){
        printf("usage : %s <port> \n",argv[0]);
        exit(1);
    }
    acpt_sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&acpt_adr,0,sizeof(acpt_adr));
    acpt_adr.sin_family=AF_INET;
    acpt_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    acpt_adr.sin_port=htons(atoi(argv[1]));

    if(bind(acpt_sock,(struct sockaddr*)&acpt_adr,sizeof(acpt_adr))==-1) 
        error_handling("bind() error");
    if(listen(acpt_sock,5)==-1)
        error_handling("listen() error");
    
    socklen_t adr_sz=sizeof(recv_adr);
    recv_sock=accept(acpt_sock,(struct sockaddr*)&recv_adr,&adr_sz);

    int str_len=0;
    while(1){
        //非阻塞IO同时PEEK查询缓冲内容 内容被读取后 缓冲中仍有
        str_len=recv(recv_sock,buf,sizeof(buf)-1,MSG_PEEK|MSG_DONTWAIT);
        if(str_len>0) 
            break;;
    }

    buf[str_len]=0;
    printf("buffer %d bytes %s\n",str_len,buf);

    str_len=recv(recv_sock,buf,sizeof(buf),0);
    printf("recv again : %s \n",buf);
    close(acpt_sock);
    close(recv_sock);
    return 0;
}

void error_handling(char *buf){
    fputs(buf,stderr);
    fputc('\n',stderr);
    exit(1);
}
