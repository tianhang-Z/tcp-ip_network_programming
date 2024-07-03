## ch4 基于TCP的服务端/客户端（1）

![image-20240612105424147](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240612105424147.png)

![image-20240612105439091](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240612105439091.png)

## ch5 基于TCP的服务端/客户端（2）

![image-20240611152653515](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240611152653515.png)	

![image-20240611152721786](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240611152721786.png)

## ch6基于UDP

![image-20240620222210838](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240620222210838.png)

![image-20240620222226006](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240620222226006.png)

## ch7优雅的断开套接字连接

![image-20240613173209232](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240613173209232.png)

![image-20240620160914234](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240620160914234.png)

传输EOF后，客户端read会返回0，即停止接受

## ch8域名系统

![image-20240613225705993](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240613225705993.png)

```
struct hostent
{
  char *h_name;			/* Official name of host.  */
  char **h_aliases;		/* Alias list.  */
  int h_addrtype;		/* Host address type.  */
  int h_length;			/* Length of address.  */
  char **h_addr_list;		/* List of addresses from name server.  */
#ifdef __USE_MISC
# define	h_addr	h_addr_list[0] /* Address, for backward compatibility.*/
#endif
};
```

## ch9套接字可选项

![image-20240614164602814](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240614164602814.png)

![image-20240614164611838](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240614164611838.png)

![image-20240614170950088](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240614170950088.png)

![image-20240614180712411](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240614180712411.png)

## ch10多进程服务器端

![image-20240618162827362](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240618162827362.png)

![image-20240618162841545](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240618162841545.png)

![image-20240618162850458](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240618162850458.png)

![image-20240619231435420](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240619231435420.png)

![image-20240620141237175](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240620141237175.png)

![image-20240620141243556](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240620141243556.png)

![image-20240620141417793](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240620141417793.png)

## ch11 进程通信

![image-20240620160453112](./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240620160453112.png)

<img src="./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240620161334784.png" alt="image-20240620161334784" style="zoom:80%;" />

<img src="./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240620161408718.png" alt="image-20240620161408718" style="zoom:80%;" />

<img src="./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240620161434714.png" alt="image-20240620161434714" style="zoom: 80%;" />

## ch12 IO复用



<img src="./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240623155134793.png" alt="image-20240623155134793" style="zoom:80%;" />

<img src="./image_tcp_ip_%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B_%E8%A6%81%E7%82%B9/image-20240623155144816.png" alt="image-20240623155144816" style="zoom:80%;" />
