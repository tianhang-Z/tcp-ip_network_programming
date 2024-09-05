#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
//ÐèÒªÁ´½Ó ws2_32.lib 
#pragma warning(disable:4996)

void error_handling(char* message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;;

	SOCKET hServerSock, hClntSock;
	SOCKADDR_IN serAddr, clntAddr;

	int szClntAddr;
	char message[] = "hello world";

	if (argc != 2) {
		printf("usage :%s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStart() error");

	hServerSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServerSock == INVALID_SOCKET)
		error_handling("socket() error");

	memset(&serAddr, 0, sizeof(serAddr));
	serAddr.sin_family = AF_INET;
	serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServerSock, (SOCKADDR*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		error_handling("bind() error");

	if (listen(hServerSock, 5) == SOCKET_ERROR)
		error_handling("listen() error");

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServerSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		error_handling("accept() error");

	send(hClntSock, message, sizeof(message), 0);
	closesocket(hServerSock);
	closesocket(hClntSock);
	WSACleanup();
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}