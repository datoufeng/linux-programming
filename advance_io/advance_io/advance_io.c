#include<sys/socket.h>
// #include<signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include"common/common.h"
#include<sys/select.h>
typedef void (*func_signal)(int);
static void connect_signal(int signal)
{
	return;
}
int connect_timeout_signal(int sockfd,const struct sockaddr_in* addr,socklen_t len,int second)
{
	func_signal* func;
	Signal(SIGALRM,connect_signal);
	alarm(second);
	int n=Connect(sockfd,addr,len);
	if(n<0)
	{
		close(sockfd);
		if(errno==EINTR)
			errno==ETIMEOUT;
	}
	alarm(0);
	Signal(SIGALRM,func);
	return n;
}

/*	利用select进行设置超时	*/
int read_time(int fd,int second)
{
	fd_set set;
	FD_ZERO(&set);
	struct timeval ti;
	ti.tv_sec=second;
	ti.tv_usec=0;
	FD_SET(fd,&set);
	return select(fd+1,&set,NULL,NULL,&ti);
}

int set_sock_timeout(int fd,int second,int u_second)
{
	struct timeval ti;
	ti.tv_sec=second;
	ti.tv_usec=u_second;	
	return setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&ti,sizeof(ti));

}

