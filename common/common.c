#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<errno.h>
#include<signal.h>
#include<unistd.h>
typedef void sigfunc(int signal);
int Read(int fd,void* ptr,int num)
{
	int left_read=num;
	int al_read=0;
	char* vptr=(char*)ptr;
	while(left_read>0)
	{
		int n=0;
		if((n=read(fd,vptr,left_read))>=0)
		{
			if(n==0)
				goto err0;
			al_read+=n;
			left_read-=n;
		}
		else if(EINTR==errno)
			continue;
		else
			return -1;
	}
err0:
	return al_read;
}

int Write(int fd,void* ptr,int num)
{
	printf("num:%d\n",num);
	int left_write=num;
	int al_write=0;
	char* vptr=(char*)ptr;
	while(left_write>0)
	{
		int n=0;
		if((n=write(fd,vptr,left_write))>=0)
		{
			printf("n:%d\n",n);
			if(n==0)
				break;
			else
			{
				left_write=left_write-n;
				al_write=al_write+n;
				vptr=vptr+n;
			}
		}
		else if(n<0&&errno==EINTR)
			continue;
		else
			return -1;
	}
	return al_write;
}

int readline(int fd,void* ptr,int max_len)
{
	int n=0;
	char* vptr=(char*)ptr;
	if(	-1!=(n=Read(fd,vptr,max_len)))
	{
		for(int i=0;i<n;i++)
		{
			if(vptr[i]=='\n')
			{
				memset(vptr+i+1,0,max_len-i-1);
				lseek(fd,-n+i+1,SEEK_CUR);
				perror("lseek");
				return i+1;
			}
		}
		return max_len;
	}
	return -1;
}


sigfunc* signal(int signal,sigfunc* func)
{
	struct sigaction act,oact;
	act.sa_handler=func;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	if(signal!=SIGALRM)
	{
		#ifdef SA_RESTART
		act.sa_flags|=SA_RESTART;
		printf("SA_RESTART\n");
		#endif
	}
	if(sigaction(signal,&act,&oact)<0)
	{
		return (SIG_ERR);
	}
	return (oact.sa_handler);
}


