#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include"common/common.h"
#include"common/tcp.h"

// #include<unistd.h>
// #include<sys/socket.h>
#include<netdb.h>
// #include<stdio.h>
#include<arpa/inet.h>
/*
	客户端 ip 和端口号 由内核自动选择（可以通过 bind() 绑定），客户端临时端口在
	第一次绑定不在更改，而 ip 会随着发送的数据报而改变
	
	如果客户端 bind() 地址，但内核从另一个数据链路发出，则 ip数据报则包含不同于外出链路 ip
	地址 的源 ip地址
	
	源ip 端口   目的ip 端口  tcp 可以获得
	对于udp 可以通过 IP_RECVDSTADDR 套接字选项获得
	
	udp  使用 connect 不会有三次握手
	
*/

int udp_echo1(int sockfd)
{
	struct sockaddr_in server;
	socklen_t ser_len=sizeof(server);
	memset(&server,0,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(9000);
	inet_pton(AF_INET,"192.168.204.253",&server.sin_addr);
	
	struct sockaddr_in reply_server;
	socklen_t reply_len=sizeof(reply_server);
	
	while(1)
	{		
		char buf[1024]="";
		int n=scanf("%s",buf);
		sendto(sockfd,(void*)buf,strlen(buf),0,(struct sockaddr*)&server,ser_len);
		
		// socklen_t ser_len=sizeof(server);
		printf("sendto:%s\n",buf);
		recvfrom(sockfd,(void*)buf,1024,0,(struct sockaddr*)&reply_server,&reply_len);/* (struct sockaddr*)&server,&ser_len */
 		if((ser_len!=reply_len)||memcpy(&server,&reply_server,sizeof(struct sockaddr_in)))
		{
			printf("err\n");
			continue;
		} 
		printf("recvfrom:%s\n",buf);
	}
}

int udp_echo2(int sockfd)	// 未实现 通过DNS查找主机名 验证主机的域名
{
 	struct sockaddr_in server;
	socklen_t ser_len=sizeof(server);
	memset(&server,0,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(9000);
	inet_pton(AF_INET,"192.168.204.253",&server.sin_addr);
	
	struct sockaddr_in reply_server;
	socklen_t reply_len=sizeof(reply_server);
	
	while(1)
	{
		
		char buf[1024]="";
		int n=scanf("%s",buf);
		sendto(sockfd,(void*)buf,strlen(buf),0,(struct sockaddr*)&server,ser_len);
		
		printf("sendto:%s\n",buf);
		alarm(5);
		recvfrom(sockfd,(void*)buf,1024,0,(struct sockaddr*)&reply_server,&reply_len);//(struct sockaddr*)&server,&ser_len 
		if(errno==EINTR)
		{
			printf("time out\n");
			break;
		}
			
		struct hostent* ex=gethostbyname("ubuntu");
		if((ex->h_length!=reply_len)||memcpy(*ex->h_addr_list,&reply_server,sizeof(struct sockaddr_in)))
		{
			printf("err\n");
			continue;
		} 
		printf("recvfrom:%s\n",buf);
	}
	

}

int udp_echo3(int sockfd)	//未实现	获取 源ip 端口  目的ip 端口  套接字选项 IP_RECVDSTADDR
{
	
}

int udp_echo4(int sockfd)	//connect 版本
{
	struct sockaddr_in server;
	server.sin_port=htons(9000);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	Connect(sockfd,(struct sockaddr*)&server,sizeof(server));
	while(1)
	{
		char buf[1024]="";
		scanf("%s",buf);		
		// printf("scanf number : %d\n",n);
		write(sockfd,buf,strlen(buf));
		printf("sendto:%s\n",buf);
		memset(buf,0,1024);
		int n=read(sockfd,buf,1024);
		if(n==4&&(strncmp("stop",buf,4)==0))
		{
			struct sockaddr_in server1;
			server1.sin_port=htons(9000);
			server1.sin_family=AF_UNSPEC;
			server1.sin_addr.s_addr=htonl(INADDR_ANY);
			connect(sockfd,(struct sockaddr*)&server1,sizeof(server1));			
		}
		printf("recvfrom:%s\n",buf);
	}
	
}

int udp_echo5(int sockfd)	//流量控制
{
	
	
}

int main(int argc,char** argv)
{
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	// udp_echo(sockfd);
	// udp_echo4(sockfd);
	udp_echo2(sockfd);

}

