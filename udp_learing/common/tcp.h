#include<sys/socket.h>
#include<errno.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
char* sock_ntop(const struct sockaddr* sa,socklen_t salen);		/*将ipv4/ipv6 地址结构转换成字符串形式*/


int Connect(int sockfd,const struct sockaddr* server_addr,socklen_t addrlen);

int Accept(int sockfd,struct sockaddr* client_addr,socklen_t* addr_len);

void Perror(const char* str);

int Bind(int sockfd,const struct sockaddr* my_addr,socklen_t addr_len);

int Listen(int fd, int backlog);

int server_bind_listen(int sock_port,const char* sock_addr,int listen_enque);


