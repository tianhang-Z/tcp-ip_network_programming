#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define BUFF_SIZE 1024
#define ECHO_SIZE 10
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

    
    char echo[ECHO_SIZE]={0};
    int op_num;
    printf("begin cal input op_num:");
    scanf("%d",&op_num);
    write(sock,&op_num,sizeof(op_num));
    read(sock,echo,ECHO_SIZE);
    if(strcmp(echo,"OK")!=0) error_handling("op_num error");

    int num=0;
    for(int i=0;i<op_num;){
        printf("input num %d :",i);
        scanf("%d",&num);
        write(sock,&num,sizeof(num));
        echo[0]=0;
        read(sock,echo,ECHO_SIZE);
        if(strcmp(echo,"OK")==0) i++;
        else error_handling("input error");
    }

    char op;
    printf("input op:");
    getchar();
    scanf("%c",&op);          //很坑 scanf读取单个字符时 他不会处理之前输入的回车
    // scanf(" %c",&op);  // %c前加空格  或者getchar清理回车
    write(sock,&op,1);
    echo[0]=0;
    read(sock,echo,ECHO_SIZE);
    if(strcmp(echo,"OK")!=0) error_handling("input error");

    int res=0;
    read(sock,&res,4);
    printf("res from server: %d\n",res);

    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}