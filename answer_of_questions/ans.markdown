### 第一章 理解[网络编程](https://so.csdn.net/so/search?q=网络编程&spm=1001.2101.3001.7020)和套接字

![img](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190401083906.png)

1. [套接字](https://so.csdn.net/so/search?q=套接字&spm=1001.2101.3001.7020)在网络编程中的作用是什么？为什么称它为套接字？

   P2，网络编程就是编写程序让两台联网的计算机相互交换数据。在我们不需要考虑物理连接的情况下，我们只需要考虑如何编写传输软件。操作系统提供了名为“套接字”，套接字是网络传输传输用的软件设备

   [socket](https://so.csdn.net/so/search?q=socket&spm=1001.2101.3001.7020)英文原意是插座：我们把插头插到插座上就能从电网获得电力供给，同样，为了与远程计算机进行数据传输，需要连接到Internet,而变成中的“套接字”就是用来连接该网络的工具

2. 在服务器端创建套接字后，会依次调用listen函数和accept函数。请比较并说明两者作用

   listen:将套接字转为可接受连接方式

   accept:受理连接请求，**并且在没有连接请求的情况调用该函数，不会返回**。直到有连接请求为止。二者存在逻辑上的先后关系

3. Linux中，对套接字数据进行I/O时可以直接使用I/O相关函数；而在Windows中则不可以。原因为何？

   Linux把套接字也看作是文件，所以可以用文件I/O相关函数；而Windows要区分套接字和文件，所以设置了特殊的函数

4. 创建套接字后一般会给它分配地址，为什么？为了完成地址分配需要调用哪些函数？

   要在网络上区分来自不同机器的套接字，所以需要地址信息。分配地址是通过`bind()`函数实现

5. Linux中的文件描述符与Windows的句柄实际上非常类似。请以套接字为对象说明他们的含义。

   Linux的文件描述符是为了区分指定文件而赋予文件的整数值（相当于编号）。Windows的文件描述符其实也是套接字的整数值，其目的也是区分指定套接字。

6. 底层文件I/O函数与ANSI标准定义的文件I/O函数之间有何区别？

   ANSI标准定义的输入、输出函数是与操作系统（内核）无关的以C标准写成的函数。相反，底层文件I/O函数是直接提供的。理论上ANSI标准I/O提供了某些机制，性能上由于底层I/O

7. 参考本书给出的示例low_open.c和low_read.c,分别利用底层文件I/O和ANSI标准I/O编写文件复制程序。可任意指定复制程序的使用方法

### 第二章：套接字类型与协议设置

1. 什么是协议？在收发数据中定义协议有何意义？

   协议就是为了完成数据交换而定好的约定。因此，定义协议意味着对数据传输所必需的的承诺进行定义。

2. 面向连接的TCP套接字传输特性有3点，请分别说明。

   - 传输过程中数据不会丢失
   - 按序传输数据
   - 传输的数据不存在数据边界(Boundary)

3. 下面哪些是面向消息的套接字的特性？a、c、e

4. UDP 、TCP 、TCP

5. 何种类型的套接字不存在数据边界？这类套接字接收数据时需要注意什么？

   连接指向型TCP套接字不存在数据边界。因此输入输出函数的响应次数不具有意义。**重要的不是函数的响应次数，而是数据的收发量**。因此，必须将传输数据的量和接收数据的量制作成编码，保证发送数据的量和接收数据的量是一致的，特别要注意是制作依赖函数响应次数判断代码

6. 修改代码

```c
/*****************************tcp_serv.c*********************************/
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include &lt;unistd.h&gt;
#include &lt;arpa/inet.h&gt;
#include &lt;sys/types.h&gt;
#include &lt;sys/socket.h&gt;

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[]="Hello World!";
	
	if(argc!=2){
		printf("Usage : %s &lt;port&gt;\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
	memset(&amp;serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &amp;serv_addr, sizeof(serv_addr))==-1)
		error_handling("bind() error"); 
	
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	clnt_addr_size=sizeof(clnt_addr);  
	clnt_sock=accept(serv_sock, (struct sockaddr*)&amp;clnt_addr,&amp;clnt_addr_size);
	if(clnt_sock==-1)
		error_handling("accept() error");  
	
	write(clnt_sock, message, 4);
	write(clnt_sock, message+4, 4);
	write(clnt_sock, message+8, 4);
	write(clnt_sock, message+12, sizeof(message)-12);

	close(clnt_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162
/*****************************tcp_clnt.c*********************************/
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include &lt;unistd.h&gt;
#include &lt;arpa/inet.h&gt;
#include &lt;sys/types.h&gt;
#include &lt;sys/socket.h&gt;

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len=0;
	int idx=0, read_len=0, i;
	
	if(argc!=3){
		printf("Usage : %s &lt;IP&gt; &lt;port&gt;\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");
	
	memset(&amp;serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
		
	if(connect(sock, (struct sockaddr*)&amp;serv_addr, sizeof(serv_addr))==-1) 
		error_handling("connect() error!");

	for(i=0; i&lt;100; i++)		// busy waiting!!
		printf("Wait time %d \n", i);

	read(sock, message, sizeof(message));
	printf("Message from server: %s \n", message);
	close(sock);
	return 0;
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950
```

### 第三章 地址族与数据序列

![img](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190401083822.png)

1. IP地址族IPv4和IPv6有何区别？在何种背景下诞生了IPv6?

   IPV4是4字节地址族，IPV6是16字节地址族。IPV6的诞生是为了应对2010年前后IP地址耗尽的问题而提出的标准

2. 通过IPV4网络ID、主机ID及路由器的关系说明向公司局域网中的计算机传输数据的过程

   首先数据传输的第一个环节是向目标IP所属的网络传输数据。此时使用的是IP地址中的网络ID。传输的数据将被传到管理网络的路由器，接受数据的路由器将参照IP地址的主机号找自己保存的路由表，找到对应的主机发送数据

3. 套接字地址分为IP地址和端口号。为什么需要IP地址和端口号？或者说，通过IP可以区分哪些对象？通过端口号可以区分哪些对象？

   IP地址是为了区分网络上的主机。端口号是区分同一主机下的不同的SOCKET，以确保软件准确收发数据。

4. CAB

5. 计算机通过路由器或交换机连接到互联网。请说出路由器和交换机的作用

   路由器是帮助数据传输到目的地的中介。不仅如此，还起到帮助连接本地网络的电脑和互联网的作用

6. 什么是知名端口？其范围是多少？知名端口中具有代表性的HTTP合同FTP端口号各是多少？

   “知名端口(Well-known PROT)”是指预定分配给特定操作的端口。其范围是0~1023，其中最知名的端口是HTTP:80端口和TCP:21

7. 题目大概意思是：为什么bind中第二个参数是`sockaddr`，但是传入的是`sockaddr_in`

bind函数第二个参数类型是`sockaddr`结构体，很难份分配IP地址和端口号，因此IP地址和PORT号的分配是通过`sockaddr_in`完成的。因为该结构体和`sockaddr`结构体的组成字节序和大小完全相同，所以可以强转

1. 请解释大端序、小端序、网络字节序，并说明为何需要网络字节序

   小端序是把高位字节存储到高位地址上；大端序是把高位字节存储到低位地址上。因为保存栈的方式有差异，所以对网络传输数据的过程制定了标准，这就是“网路字节序”。而且，在网络字节序中，数据传输的标准是“大端序”

2. 大端计算机希望将4字节整型数据12传到小端序计算机。请说出数据传输过程中发生的字节序变换过程

   因为网络字节序的顺序标准是“大端序”，所以大端序的计算机在网络传输中不需要先转换字节顺序，直接传输。但是接受数据的是小端序计算机，因此，要经过网络转本地序的过程，再保存到存储设备上

3. 怎么表示回送地址？其含义是什么？如果向回送地址传输数据将会发生什么情况？

   回送地址表示计算机本身，为`127.0.0.1`。因此，如果将数据传送到IP地址`127.0.0.1`，数据会不会通过传输到网络的其他设备上而直接返回。

### 第四章 基于TCP的服务器端/客户端（1）

1. 请说明TCP/IP的4层协议栈，并说明TCP和UDP套接字经过的层级结构差异

   ![img](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190401091525.png)

   链路层—>IP层—>TCP层—>应用层

   链路层—>IP层—>UDP层—>应用层

2. 请说出TCP/IP协议栈中链路层和IP层的作用，并给出两者关系。

   链路层是LAN、WAN、MAN等网络标准相关的协议栈，是定义物理性质标准的层级。相反，IP层是定义网络传输数据标准的层级。即IP层负责以链路层为基础的数据传输

3. 为何需要把TCP/IP协议栈分成4层（或7层）？结合开放式系统回答

   将复杂的TCP/IP协议分层化的话，就可以将分层的层级标准发展成开放系统。实际上，TCP/IP是开放系统，各层级都被初始化，并以该标准为依据组成了互联网。因此，按照不同层级标准，硬件和软件可以相互替代，这种标准化是TCP/IP蓬勃发张的依据

4. 客户端调用connect函数向服务器端发送连接请求。服务器端调用哪个函数后，客户端可以调用connect函数？

   listen函数，客户端才可以调用connect函数

5. 什么时候创建连接请求等待队列？它有何作用？与accept有什么关系

   listen函数的调用创建了请求等待队列。它是存储客户端连接请求信息的空间。accept函数调用后，将从本地存储的连接请求信息取出，与客户端建立连接。

6. 客户端中为何不需要调用bind函数分配地址？如果不调用bind函数，那何时、如何向套接字分配IP地址和端口号？

   客户端是请求连接的程序，不是一个接收连接的程序。所以，指导服务器的地址信息是更重要的因素，没有必要通过bind函数明确地分配地址信息。但是，要想和服务器通信，必须将自己的地址信息分配到套接字上，因此，在connect函数调用时，自动把IP地址和端口号输入到套接字上

7. 改成迭代服务器端

```c
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include &lt;unistd.h&gt;
#include &lt;arpa/inet.h&gt;
#include &lt;sys/socket.h&gt;

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[]="Hello World!";
	
	if(argc!=2){
		printf("Usage : %s &lt;port&gt;\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
	memset(&amp;serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
	
	if( bind(serv_sock, (struct sockaddr*) &amp;serv_addr, sizeof(serv_addr))==-1 )
		error_handling("bind() error"); 
	
	if( listen(serv_sock, 5)==-1 )
		error_handling("listen() error");
	
	clnt_addr_size=sizeof(clnt_addr);  
	
	while(1)
	{
		clnt_sock=accept(serv_sock, (struct sockaddr*)&amp;clnt_addr,&amp;clnt_addr_size);
		if(clnt_sock==-1)
			break;  
		
		write(clnt_sock, message, sizeof(message));
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061
```

### 第五章 基于TCP的服务器端/客户端（2）

1. 请说明TCP套接字连接设置的三次握手过程。尤其是3次数据交换过程每次收发的数据内容。

   ![img](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190401094016.png)

   网上有很多，就不一一写了

2. TCP是可靠的数据传输协议，但在通过网络通信的过程可能丢失数据。请通过ACK和SEQ说明TCP通过何种机制保证丢失数据的可靠传输。

   SEQ顺序标识符是给信息编号。ACK是用于回复带有编号的信息。也就是说，每次传输信息时，都同时发送SEQ标识，而受到信息的主机应以SEQ信息为基础回复发送信息的主机。通过这种机制，传输数据的主机就可以确认数据是否被正确接收。在传输失败时，可以重新传送。

3. TCP套接字中调用write和read函数时数据如何移动？结合I/O缓冲进行说明

   当write函数被调用时，数据就会向端口的输出缓冲区移动。然后经过网络传输传输到对方主机套接字的输入缓冲。这样，输入缓冲中存储的数据通过read函数的响应来读取

4. 对方主机的输入缓冲剩余50字节空间时，若本方主机通过write函数请求传输70字节，问TCP如何处理这种情况？

   对方主机会把输入缓冲中可存储的数据大小传送给要传输数据的数据（本方）。因此，在剩余空间为50字节的情况，即使要求传送70字节的数据，也不能传输50字节以上，剩余的部分保存在传输方的输出缓冲中，等待对方主机的输入缓冲出现空间。而且，这种交换缓冲多余空间信息的协议被称为滑动窗口协议

5. 更改程序，使服务器端和客户端各传送1次字符串…

```c
/**********************************sendrecv_serv.c***********************************/
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include &lt;unistd.h&gt;
#include &lt;arpa/inet.h&gt;
#include &lt;sys/types.h&gt;
#include &lt;sys/socket.h&gt;

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
	int str_len, i;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char msg1[]="Hello client!";
	char msg2[]="I'm server.";
	char msg3[]="Nice to meet you.";
	char* str_arr[]={msg1, msg2, msg3};
	char read_buf[100];
	
	if(argc!=2){
		printf("Usage : %s &lt;port&gt;\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
	memset(&amp;serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&amp;serv_addr, sizeof(serv_addr))==-1)
		error_handling("bind() error"); 
	
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	clnt_addr_size=sizeof(clnt_addr);  
	clnt_sock=accept(serv_sock, (struct sockaddr*)&amp;clnt_addr,&amp;clnt_addr_size);
	if(clnt_sock==-1)
		error_handling("accept() error");  
	
	for(i=0; i&lt;3; i++)
	{
		str_len=strlen(str_arr[i])+1;
		write(clnt_sock, (char*)(&amp;str_len), 4);
		write(clnt_sock, str_arr[i], str_len);
		
		read(clnt_sock, (char*)(&amp;str_len), 4);
		read(clnt_sock, read_buf, str_len);
		puts(read_buf);
	}
	
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768697071727374
/***************************************recvsend_clnt.c***************************/
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include &lt;unistd.h&gt;
#include &lt;arpa/inet.h&gt;
#include &lt;sys/types.h&gt;
#include &lt;sys/socket.h&gt;

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;

	char msg1[]="Hello server!";
	char msg2[]="I'm client.";
	char msg3[]="Nice to meet you too!";
	char* str_arr[]={msg1, msg2, msg3};
	char read_buf[100];

	int str_len, i;
	
	if(argc!=3){
		printf("Usage : %s &lt;IP&gt; &lt;port&gt;\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");
	
	memset(&amp;serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&amp;serv_addr, sizeof(serv_addr))==-1) 
		error_handling("connect() error!");

	for(i=0; i&lt;3; i++)
	{
		read(sock, (char*)(&amp;str_len), 4);
		read(sock, read_buf, str_len);
		puts(read_buf);

		str_len=strlen(str_arr[i])+1;
		write(sock, (char*)(&amp;str_len), 4);
		write(sock, str_arr[i], str_len);
	}
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162
```

1. 创建收发文件的服务器端/客户端

```c
/**********************************file_serv.c***********************************/
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include &lt;unistd.h&gt;
#include &lt;arpa/inet.h&gt;
#include &lt;sys/socket.h&gt;

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sd, clnt_sd;
	FILE * fp;
	char buf[BUF_SIZE];
	char file_name[BUF_SIZE];
	int read_cnt;
	
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	
	if(argc!=2) {
		printf("Usage: %s &lt;port&gt;\n", argv[0]);
		exit(1);
	}
	
	serv_sd=socket(PF_INET, SOCK_STREAM, 0);   
	
	memset(&amp;serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	bind(serv_sd, (struct sockaddr*)&amp;serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);
	
	clnt_adr_sz=sizeof(clnt_adr);    
	clnt_sd=accept(serv_sd, (struct sockaddr*)&amp;clnt_adr, &amp;clnt_adr_sz);
	
	read(clnt_sd, file_name, BUF_SIZE);
	fp=fopen(file_name, "rb");
	if(fp!=NULL)
	{
		while(1)
		{
			read_cnt=fread((void*)buf, 1, BUF_SIZE, fp);
			if(read_cnt&lt;BUF_SIZE)
			{
				write(clnt_sd, buf, read_cnt);
				break;
			}
			write(clnt_sd, buf, BUF_SIZE);
		}
	}
	
	fclose(fp);
	close(clnt_sd); close(serv_sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768
/***************************************recvsend_clnt.c***************************/
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include &lt;unistd.h&gt;
#include &lt;arpa/inet.h&gt;
#include &lt;sys/socket.h&gt;

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sd;
	FILE *fp;
	
	char buf[BUF_SIZE];
	char file_name[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_adr;
	if(argc!=3) {
		printf("Usage: %s &lt;IP&gt; &lt;port&gt;\n", argv[0]);
		exit(1);
	}
	
	printf("Input file name: ");
	scanf("%s", file_name);
	fp=fopen(file_name, "wb");

	sd=socket(PF_INET, SOCK_STREAM, 0);   
	memset(&amp;serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	connect(sd, (struct sockaddr*)&amp;serv_adr, sizeof(serv_adr));
	write(sd, file_name, strlen(file_name)+1);	

	while((read_cnt=read(sd, buf, BUF_SIZE))!=0)
		fwrite((void*)buf, 1, read_cnt, fp);
	
	fclose(fp);
	close(sd);
	return 0;
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152
```

### 第六章 基于UDP的服务器端/客户端

1. UDP为什么比TCP速度快？为什么TCP数据传输可靠而UDP数据传输不可靠？

   UDP和TCP不同，不进行流量控制。由于该控制涉及到套接字的连接和结束，以及整个数据收发过程，因此，TCP传输的数据是可以信赖的。相反,UDP不进行这种控制，因此无法信任数据的传输，但正因UDP不进行流量控制，所以比TCP更快

2. bce

3. UDP数据包向对方主机的UDP套接字传递过程中，IP和UDP分别负责哪些部分？

   IP负责链路选择。UDP负责端到端的传输

4. UDP一般比TCP快，但根据交换数据的特点，其差异可大可小。请说明何种情况下UDP的性能优于TCP

   UDP与TCP不同，不经过连接以及断开SOCKET的过程，因此，在频繁的连接及断开的情况下，UDP的数据收发能力会凸显出更好的性能。

5. 客户端TCP套接字调用connect函数时自动分配IP和端口号。UDP中不调用bind函数，那何时分配IP和端口号？

   首次调用sendto函数时，发现尚未分配信息，则给相应的套接字自动分配IP和端口号

6. TCP客户端必须调用connect函数，而UDP中可以选择性调用。请问，在UDP中调用connect函数有哪些好处？

   每当以UDP套接字为对像调用sendto函数时，都要经过以下过程

   - 第一阶段：为目标UDP注册端口和IP
   - 第二阶段：数据传输
   - 第三阶段：删除UDP注册的IP和端口信息

   其中，只要调用connect函数，就可以忽略每次传输数据时反复进行的第一阶段和第三阶段。然而，调用connect函数并不意味着经过连接过程，只是将IP地址和端口号指定在UDP的发送对象上。这样connect函数使用后，还可以用write、read函数进行数据处理，而不必使用sendto、recvfrom

7. 收发的消息均要输出到控制台窗口

```c
/********************************uchar_server.c***********************************/
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include &lt;unistd.h&gt;
#include &lt;arpa/inet.h&gt;
#include &lt;sys/socket.h&gt;

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	char message[BUF_SIZE];
	int str_len;
	socklen_t clnt_adr_sz;
	
	struct sockaddr_in serv_adr, clnt_adr;
	if(argc!=2){
		printf("Usage : %s &lt;port&gt;\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_DGRAM, 0);
	if(serv_sock==-1)
		error_handling("UDP socket creation error");
	
	memset(&amp;serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&amp;serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	
	clnt_adr_sz=sizeof(clnt_adr);
	while(1) 
	{
		str_len=recvfrom(serv_sock, message, BUF_SIZE, 0, 
								(struct sockaddr*)&amp;clnt_adr, &amp;clnt_adr_sz);
		message[str_len]=0;
		printf("Message from client: %s", message);

		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))	
			break;

		sendto(serv_sock, message, strlen(message), 0, 
								(struct sockaddr*)&amp;clnt_adr, clnt_adr_sz);
	}	
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263
/********************************uchar_client.c***********************************/
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include &lt;unistd.h&gt;
#include &lt;arpa/inet.h&gt;
#include &lt;sys/socket.h&gt;

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	socklen_t adr_sz;
	
	struct sockaddr_in serv_adr, from_adr;
	if(argc!=3){
		printf("Usage : %s &lt;IP&gt; &lt;port&gt;\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_DGRAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&amp;serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	while(1)
	{
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);     
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))	
			break;
		
		sendto(sock, message, strlen(message), 0, 
					(struct sockaddr*)&amp;serv_adr, sizeof(serv_adr));
		adr_sz=sizeof(from_adr);
		str_len=recvfrom(sock, message, BUF_SIZE, 0, 
					(struct sockaddr*)&amp;from_adr, &amp;adr_sz);

		message[str_len]=0;
		printf("Message from server: %s", message);
	}	
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859
```

### 第七章 优雅地断开套接字连接

1. 解释TCP中“流”的概念。UDP中能否形成流？请说明原因

   TCP的流指，两台主机通过套接字建立连接后进入可交换数据的状态，也称为“流形成的状态”。而对于UDP来说，不存在流，因为两个SOCKET不能相互连接

2. Linux中的close函数或Windows中的`closesocket`函数属于单方面断开连接的方法，有可能带来一些问题。什么是单方面断开连接？什么情况下会出现问题？

   单方面的断开连接意味着套接字无法再发送数据。一般在对方有剩余数据为发送完成时，断开己方连接，会造成问题。

3. 什么是半关闭？针对输出流执行半关闭的主机处于何种状态？半关闭会导致对方主机接收什么信息？

   半关闭是指只完成输入和输出流中的一个。而且，如果对输出流进行半关闭，EOF无法被传送到对方主机，己方套接字无法传送数据，但可以接收对方主机传送的数据。

### 第八章 域名及网络地址

1. bd

2. …

   如果网络上没有特别的限制，可以将与本地网络相连的DNS服务器指定为其他完好的DNS服务器。因此，东秀提议的方法可能成为解决方法。也就是说，静洙可以不去网吧

3. …

   - 计算机向DNS服务器询问IP地址
   - 默认DNS服务器没有IP地址信息，因此向DNS主机发出询问
   - DNS查询服务器从其更上级的DNS服务器接收IP地址信息
   - DNS查询服务器将查到的IP地址逐级返还给主机
   - 网络浏览器访问接收到的IP地址网站

### 第九章 套接字的多种可选项

1. 下列关于Time-wait状态的说法错误的是？acd(作者的答案就acd,至于d选项是否有问题，见仁见智)

2. TCP_NODELAY可选项与Nagle算法有关，可通过它禁止Nagle算法。请问何时应考虑禁用Nagle算法？结合收发数据的特性给出说明

   根据传输数据的特性，网络流量未受太大影响时，不使用Nagle算法要比使用它时传输速度快。例如“传输大文件数据”。将文件数据传入输出缓冲不会花太多时间，因此，即便不使用Nagle算法那，也会在装满输出缓冲时传输数据包。这不仅不会增加数据包的数量，反而会在无需等待ACK的前提下连续传输，因此可以大大提高传输速度。

### 第十章 多进程服务器端

![img](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190401142353.png)

1. 下列关于进程的说法错误的是？cd
2. 调用fork函数将创建子进程，以下关于子进程描述错误的是？acd(a选项我测试过，真的不会销毁)
3. 创建子进程时将复制父进程的所有内容，此时的复制对象也包含套接字文件描述符。编写程序验证复制的文件描述符整数值是否与原文件描述符整数值相同。

```c
#include &lt;unistd.h&gt;
#include &lt;sys/socket.h&gt;

int main(int argc, char *argv[])
{
	pid_t pid;
	int sockfd=socket(PF_INET, SOCK_STREAM, 0);
	
	pid=fork();		
	
	if(pid==0)
		printf("Child sock fd: [%d] \n", sockfd);
	else
		printf("Parent sock fd: [%d] \n", sockfd);
	return 0;
}
12345678910111213141516
```

显示
Parent sock fd: [3]
Child sock fd: [3]

- 请说明进程变为僵尸进程的过程及预防措施

  僵尸进程是子进程。在子进程结束时，其返回值会传到操作系统，直到返回值被其父进程接收为止，该（子）进程会一直作为僵尸进程存在。所以，为了防止这种情况的发生，父进程必须明确接收子进程结束时的返回值。

- 编写程序使其每隔1秒输出简单字符串，并适用于上述时间处理器注册代码

- ```c
  #include &lt;stdlib.h&gt;
  #include &lt;signal.h&gt;
  
  void ctrl_handler(int sig);
  
  int main(int argc, char *argv[])
  {
  	struct sigaction act;
  	act.sa_handler=ctrl_handler;
  	sigemptyset(&amp;act.sa_mask);
  	act.sa_flags=0;
  	sigaction(SIGINT, &amp;act, 0);
  
  	while(1)
  	{
  		sleep(1);
  		puts("Have a nice day~");
  	}
  
  	return 0;
  }
  
  
  void ctrl_handler(int sig)
  {
  	char ex;
  	fputs("Do you want exit(Y to exit)? ", stdout);
  	scanf("%c", &amp;ex);
  	if(ex=='y' || ex=='Y')
  		exit(1);
  }
  
  1234567891011121314151617181920212223242526272829303132
  ```

  ### 第十一章 进程间通信

  1. 什么是进程间通信？分别从概念和内存的角度进行说明

     概括性地说，进程间通信是指两个进程之间交换数据。但是从内存的角度看，可以理解为两个进程共有内存。因为共享的内存区域存在，可以进行数据交换

  2. 进程间通信需要特殊的IPC机制，这是由操作系统提供的。进程间通信时为何需要操作系统的帮助？

     要想实现IPC机制，需要共享的内存，但由于两个进程之间不共享内存，因此需要操作系统的帮助，也就是说，两进程共享的内存空间必须由操作系统来提供

  3. “管道”是典型的IPC技术。关于管道，请回答如下问题。

     - 管道是进程间交换数据的路径。如何创建该路径?由谁创建？

       管道是由pip函数产生的，而实际产生管道的主体是操作系统

     - 为了完成进程间通信，2个进程需同时连接管道。那2个进程如何连接到同一管道？

       pipe函数通过输入参数返回管道的输入输出文件描述符。这个文件描述符在fork函数中复制到了其子进程，因此，父进程和子进程可以同时访问同一管道。

     - 管道允许进行2个进程间的双向通信。双向通信中需要注意哪些内容？

       管道并不管理进程间的数据通信。因此，如果数据流入管道，任何进程都可以读取数据。因此，要合理安排共享空间的输入和读取

  4. 编写示例复习IPC技术，使2个进程相互交换3次字符串…

  ```c
  #include &lt;string.h&gt;
  #include &lt;unistd.h&gt;
  #define BUF_SIZE 30
  
  int main(int argc, char *argv[])
  {
  	int fds1[2], fds2[2];
  	char str1[]="Do you like coffee?";
  	char str2[]="I like coffee";
  	char str3[]="I like bread";
  	char * str_arr[]={str1, str2, str3};
  	char buf[BUF_SIZE];
  	pid_t pid;
  	int i;
  	
  	pipe(fds1), pipe(fds2);
  	pid=fork();
  	
  	if(pid==0)
  	{
  		for(i=0; i&lt;3; i++)
  		{
  			write(fds1[1], str_arr[i], strlen(str_arr[i])+1);
  			read(fds2[0], buf, BUF_SIZE);
  			printf("Child proc output: %s \n",  buf);
  		}
  	}
  	else
  	{
  		for(i=0; i&lt;3; i++)
  		{
  			read(fds1[0], buf, BUF_SIZE);
  			printf("Parent proc output: %s \n", buf);
  			write(fds2[1], str_arr[i], strlen(str_arr[i])+1);
  		}
  	}
  	return 0;
  }
  1234567891011121314151617181920212223242526272829303132333435363738
  ```

  ### 第十二章 I/O复用

  1. 请解释复用技术的通用含义，并说明何为I/O复用。

     复用技术指为了提高物理设备的效率，用最少的物理要素传递最多数据时使用的技术。同样，I/O复用是指将需要I/O的套接字捆绑在一起，利用最少限度的资源来收发数据的技术

  2. 多进程并发服务器的缺点有哪些？如何在I/O复用服务器端中弥补？

     多进程并发服务器的服务方式是，每当客户端提出连接要求时，就会追加生成进程。但构建进程是一项非常有负担的工作，因此，向众多客户端提供服务存在一定的局限性。而复用服务器则是将套接字的文件描述符捆绑在一起管理的方式，因此可以一个进程管理所有的I/O操作

  3. bc

  4. select函数的观察对象中应包含服务器端套接字（监听套接字），那么应将其包含到哪一类监听对象集合？请说明原因

     服务器套接字的作用是监听有无连接请求，即判断接收的连接请求是否存在？应该将其包含到“读”类监听对象的集合中

  5. select函数使用的fd_set结构体在Windows和Linux中具有不同的声明。请说明却别，同时解释存在区别的必然性

     Linux的文件描述符从0开始递增，因此可以找出当前文件描述符数量和最后生成的文件描述符之间的关系。但Windows的套接字句柄并非从0开始，并且句柄的整数值之间并无规律可循，因此需要直接保存句柄的数组和记录句柄数的变量

  ### 第十三章 多种I/O函数

  1. 下面关于MSG_OOB可选项的说法错误的时？bc

  2. 利用readv&writev函数收发数据有何优点？分别从函数调用次数和I/O缓冲的角度给出说明

     readv&writev函数可以将分散保存在多个缓冲中的数据一并接受和发送，是对数据进行整合传输及发送的函数，因此可以进行更有效的数据传输。而且，输入输出函数的调用次数也相应减少，也会产生相应的优势

  3. 通过recv函数见证输入缓冲是否存在数据时（确认后立即返回），如何设置recv函数最后一个参数中的可选项？分别说明各可选项的含义

     同时设置MSG_PEEK选项和MSG_DONTWAIT选项，以验证输入缓冲是否存在可接收的数据。设置MSG_PEEK选项并调用recv函数时，即使读取了输入缓冲数据也不会删除。因此，该选项通常与MSG_DONTWAIT合作，用于调用以非阻塞方式验证待读数据存在与否的函数。

  4. 可在Linux平台通过注册时间处理函数接收MSG_OOB数据。那Windows中如何接受？请说明接收方法

     MSG_OOB数据的接收，在select函数中属于异常数据，既在Windows中可以通过异常处理来接收Out-of-band数据

  ### 第十四章 多播与广播

  1. TTL的含义是什么？请从路由器的角度说明较大的TTL值与较小的TTL值之间的区别及问题

     TTL是Time to Live的缩写，是决定“信息传递距离”的主要因素。TTL表现为整数，没经过一个路由器就减1。如果该值为0，该数据报就会因无法再传递而消失。TTL设置大了会对网络流量造成不良影响，设置太小的话，就可能无法到达目的地

  2. 多播和广播的异同是什么？请从数据通信的角度进行说明

     多播和广播的相同点是，两者都是以UDP形式传输数据。一次传输数据，可以向两个以上主机传送数据。但传送的范围是不同的：广播是对局域网的广播；而多播是对网络注册机器的多播

  3. bc

  4. 多播也对网络流量有利，请比较TCP数据交换方式解释其原因。

     多播数据在路由器进行复制。因此，即使主机数量很多，如果各主机存在的相同路径，也可以通过一次数据传输到多台主机上。但TCP无论路径如何，都要根据主机数量进行数据传输。

  5. 多播方式的数据通信需要MBone虚拟网络。换言之，MBone是用于多播的网络，但它是虚拟网络。请解释此处“虚拟网络”。

     以物理网络为基础，通过软件方法实现的多播通信必备的虚拟网络