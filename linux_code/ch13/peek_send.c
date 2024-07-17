#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *buf);

int main(int argc,char* argv[]){

    int sock;
    struct sockaddr_in send_adr;
    if(argc!=3){
        printf("usage : %s <ip> <port> \n",argv[0]);
        exit(1);
    }

    sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&send_adr,0,sizeof(send_adr));
    send_adr.sin_family=AF_INET;
    send_adr.sin_addr.s_addr=inet_addr(argv[1]);
    send_adr.sin_port=htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&send_adr,sizeof(send_adr))==-1)
        error_handling("conenct() error");
    
    write(sock,"123",strlen("123"));

    close(sock);
    return 0;

}

void error_handling(char *buf){
    fputs(buf,stderr);
    fputc('\n',stderr);
    exit(1);
}
