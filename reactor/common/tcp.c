#include "tcp.h"
typedef struct sockaddr SA;
void Perror(const char* str)
{
	perror(str);
	exit(-1);
}

int Connect(int sockfd,const struct sockaddr* serveraddr,socklen_t addrlen)
{
	int status=connect(sockfd,serveraddr,addrlen);		//connect()	阻塞，仅在连接成功或者出错时返回
	int flag=fcntl(sockfd,F_GETFL,0);					//connect() 失败时，不能立刻重新连接
	if((flag&O_NONBLOCK)!=O_NONBLOCK)
		printf("sockfd block flag\n");
	if(flag<0)
	{
		Perror("Connect err");
	}
	return status;
}

int Accept(int sockfd,struct sockaddr* client_addr,socklen_t* addr_len)
{
	int status=0;
again:
	status=accept(sockfd,client_addr,addr_len);
	if(status<0)
	{
		if(errno==EINTR||errno==ECONNABORTED)
			goto again;
		else
		{
			printf("epoll_accept\n");
			Perror("Accept");
		}
	}
	return status;
}

int Bind(int sockfd,const struct sockaddr* my_addr,socklen_t addr_len)
{
	int status=bind(sockfd,my_addr,addr_len);
	if(status<0)
		Perror("Bind");
	return status;
}

int Listen(int fd, int backlog)
{
    int status=0;

	if ((status = listen(fd, backlog)) < 0)
		Perror("listen");
    return status;
}

char* auto_inet_ntop(const struct sockaddr* sa,socklen_t salen)	/* 将ipv4/ipv6 地址结构转换成字符串形式 */
{
	char* str=(char*)malloc(sizeof(128));
	if(sa->sa_family==AF_INET)
	{
		struct sockaddr_in* sin=(struct sockaddr_in*)sa;
		if(inet_ntop(AF_INET,&sin->sin_addr,str,128)==NULL)
			return NULL;
		return str;
	}
	else if(sa->sa_family==AF_INET6)
	{
		struct sockaddr_in6* sin=(struct sockaddr_in6*)sa;
		if(inet_ntop(AF_INET6,&sin->sin6_addr,str,128)==NULL)
			return NULL;
		return str;		
	}
}

int server_bind_listen(int sock_port,char* sock_addr,int listen_enque)
{
	int n;
	int listen_fd=-1;
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(sock_port);
	if(sock_addr==NULL)
	{
		server.sin_addr.s_addr=htonl(INADDR_ANY);
	}
	else
	{
		if((n=inet_pton(AF_INET,sock_addr,&server.sin_addr))==0)
		{
			Perror("input format err");
		}
		else if(n==-1)
		{
			Perror("err inet_pton");
		}
	}
	listen_fd=socket(AF_INET,SOCK_STREAM,0);
	if(listen_fd==-1)
		Perror("socket");
	n=bind(listen_fd,(SA*)&server,sizeof(server));
	if(n!=0)
		Perror("bind");
	if((n=listen(listen_fd,listen_enque))!=0)
		Perror("listen");
	
	return listen_fd;
}

