#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUFF_SIZE 100
#define EPOLL_SIZE 50

void error_handling(char *message);
void read_childproc(int sig);

//基于epoll的IO复用服务器端
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

    //epoll相关变量
    struct epoll_event *ep_event;  //用于设置epoll_wait的event参数 ，保存监视的结果
    struct epoll_event event;  //用于设置epoll_ctl的event参数
    int epfd,event_cnt;

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
        
    //epoll配置
    epfd=epoll_create(EPOLL_SIZE);   //向OS申请epoll描述符的空间
    ep_event=malloc(sizeof(struct epoll_event)*EPOLL_SIZE);  //为ep_event申请空间
    event.events=EPOLLIN;
    event.data.fd=serv_sock;
    //在epfd中注册serv_sock 监视event中的事件
    epoll_ctl(epfd,EPOLL_CTL_ADD,serv_sock,&event); 

    printf("serv_sock fd :%d \n",serv_sock);
    while (1)
    {
        //等待事件发生
        event_cnt=epoll_wait(epfd,ep_event,EPOLL_SIZE,-1);
        if(event_cnt==-1){
            puts("epoll_wait() error");
            break;
        }   

        //依次处理事件
        for(int i=0;i<event_cnt;i++){
            //serv_sock 收到数据 扩展epfd监视范围
            if(ep_event[i].data.fd==serv_sock){
                clnt_adr_sz=sizeof(clnt_adr);
                clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
                event.events=EPOLLIN;
                event.data.fd=clnt_sock;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sock,&event); 
                printf("new connect clnt sock :%d \n",clnt_sock);
            }
            //其他sock收到数据 回声服务
            else{
                str_len=read(ep_event[i].data.fd,message,BUFF_SIZE);
                //EOF  关闭对应clnt_sock
                if(str_len==0) {
                    epoll_ctl(epfd,EPOLL_CTL_DEL,ep_event[i].data.fd,NULL); 
                    close(ep_event[i].data.fd);
                    printf("close clnt sock:%d \n",ep_event[i].data.fd);
                }
                //回声
                else{
                    printf("read str_len:%d \n",str_len);
                    write(ep_event[i].data.fd,message,str_len);
                }  
            }
        }
    }
       
    
    close(serv_sock);
    close(epfd);  //epfd也要关闭
    return 0;
}


void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}