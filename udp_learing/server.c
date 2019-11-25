#include<sys/socket.h>
#include<stdio.h>
#include<unistd.h>
#include"common/common.h"
#include"common/tcp.h"
/*对于无连接套接字，他引发的错误并不返回给他，仅仅在用 connect()  连接是才会发生，错误返回*/
/* recvfrom 最后一个参数 值-结果参数，必须赋予值 */

int udp_echo(int sockfd/* ,struct sockaddr* client,socklen_t* cli_len */)
{
	int k=0;
	struct sockaddr client;
	while(1)
	{
		socklen_t cli_len=sizeof(struct sockaddr_in);
		char buf[1024]="";
		int n=recvfrom(sockfd,(void*)buf,1024,0,(struct sockaddr*)&client,&cli_len);	/* 最后一个参数 值-结果参数，必须赋予值 */
		printf("recvfrom:%s\n",buf);
		if(k++>3)
		{
			sleep(10);
		}
		
		sendto(sockfd,(void*)buf,n,0,(struct sockaddr*)&client,cli_len);
		printf("sendto:%s\n",buf);
		
		memset(&client,0,sizeof(client));
	}
}

int main(int argc,char** argv)
{
	struct sockaddr_in server;
	memset(&server,0,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(9000);
	// &server.sin_addr);
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	int i=Bind(sockfd,(struct sockaddr*)&server,sizeof(server));
	
	udp_echo(sockfd);
	
	
}


