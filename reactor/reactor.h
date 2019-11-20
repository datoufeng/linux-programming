#include<iostream>
#include<list>
#include<sys/epoll.h>
#include"common/common.h"
#include"common/tcp.h"
using namespace std;

typedef int (*func)(void* arg);

struct data
{
	int epfd;
	bool flag=false;
	int fd;
	func call_back;
	void* arg;
	struct sockaddr_in client;
	int sock_len;
};

class reactor
{
	private:
	int epfd;
	list<data*> reactor_list;
	
	public:
	reactor();
	reactor(char* ip_addr="127.0.0.1",int port=9000);
	~reactor();
	
	bool init(char* ip_addr="127.0.0.1",int port=9000);
	bool fini();
	
	void run();
	
	bool check_event_exit(int fd,int events);
	bool add_event(int fd,int events,func events_act);
	bool del_event(int fd,int events,func events_act);

};

