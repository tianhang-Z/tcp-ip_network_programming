#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

void error_handling(char* message);

int main(int argc,char* argv[]) {
	WSADATA wsaData;
	SOCKET hsocket;

	SOCKADDR_IN serAddr;

	char message[30];
	int str_len;
	if (argc != 3) {
		printf("usage %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error");

	hsocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hsocket == INVALID_SOCKET)
		error_handling("socket() error");
	
	memset(&serAddr, 0, sizeof(serAddr));
	serAddr.sin_family = AF_INET;
	serAddr.sin_addr.s_addr = inet_addr(argv[1]);
	serAddr.sin_port = htons(atoi(argv[2]));

	if(connect(hsocket,(SOCKADDR*) &serAddr,sizeof(serAddr))==SOCKET_ERROR)
		error_handling("connect() error");
	
	str_len = recv(hsocket, message, sizeof(message)-1,0);
	if (str_len == -1)
		error_handling("recv() error");
	else
		printf("message from server : %s \n", message);
	
	closesocket(hsocket);
	WSACleanup();
	return 0;

}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

