#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *buf);

// 基于IO复用的服务端

int main(int argc,char *argv[]){

    if(argc!=2){
        printf("usage :%s <port>\n",argv[0]);
        exit(1);
    }

    int serv_sock,clnt_sock;
    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t adr_sz;

    struct timeval timeout;
    int fd_max,fd_num;
    fd_set reads,cpy_reads;
    
    char buf[BUF_SIZE];

    //服务端配置socket bind listen accept
    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1) error_handling("socket() error\n");
    else printf("serv_sock :%d \n",serv_sock);
    
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("bind() error");
    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");

    FD_ZERO(&reads);
    FD_SET(serv_sock,&reads);
    fd_max=serv_sock;

    while(1){
        cpy_reads=reads;
        timeout.tv_sec=5;
        timeout.tv_usec=5000;

        //select进行输入监视
        if((fd_num=select(fd_max+1,&cpy_reads,0,0,&timeout))==-1)
            break;
        if(fd_num==0)
            continue;
        
        for(int i=0;i<fd_max+1;i++){
            //对于监视的范围 选出有输入信号的
            if(FD_ISSET(i,&cpy_reads)){
                //serv_sock 有输入 说明有新的连接请求 则fd_set扩展
                if(i==serv_sock){
                    adr_sz=sizeof(clnt_adr);
                    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&adr_sz);
                    FD_SET(clnt_sock,&reads);
                    if(fd_max<clnt_sock) fd_max=clnt_sock;
                    printf("connect client :%d \n",clnt_sock);
                }
                //其他有输入 说明有客户端消息
                else{
                    int str_len;
                    str_len=read(i,buf,BUF_SIZE);   //收到EOF时 str_len为0 此时要断开clnt_sock
                    if(str_len==0){
                        FD_CLR(i,&reads);
                        close(i);
                        printf("close client %d \n",i);
                    }
                    else{
                        write(i,buf,str_len);
                    }

                }
            }
        }
    }



}

void error_handling(char *buf){
    fputs(buf,stderr);
    fputc('\n',stderr);
    exit(1);
}
