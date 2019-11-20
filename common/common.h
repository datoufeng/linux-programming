#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
typedef void sigfunc(int signal);
int Read(int fd,void* ptr,int num);		/*读取n个字节*/

int readline(int fd,void* ptr,int max_len);		/*读取一行内容*/

int Write(int fd,void* ptr,int num);		/**/

sigfunc* signal(int signal,sigfunc* func);


