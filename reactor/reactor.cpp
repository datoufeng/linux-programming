#include"reactor.h"

reactor::reactor()
{
	this->epfd=epoll_create(1);
}

reactor::reactor(char* ip_addr="127.0.0.1",int port=9000,func func_back)
{
	this->epfd=epoll_create(1);
	init(ip_addr,port,func_back);
}

reactor::~reactor()
{
	fini();
}

bool reactor::init(char* ip_addr="127.0.0.1",int port=9000,func func_back=react_accept)
{
	struct epoll_event en;
	data* msg=new data;
	int lis_fd=server_bind_listen(ip_addr,port);
	
	msg->fd=lis_fd;
	msg->call_back=func_back;
	msg->epfd=epfd;
	
	en.events=EPOLLIN;
	en.data.ptr=msg;
	
	epoll_ctl(this->epfd,EPOLL_CTL_ADD,lis_fd,&en);
	reactor_list.append(msg);
}

bool reactor::fini()
{
	while(reactor_list.size()!=0)
	{
		auto item=reactor_list.front();
		reactor_list.pop_front();
		delete item;
	}
}

void reactor::run()
{
	struct epoll_event ens[1024],en;
	while(1)
	{
		int num=epoll_wait(epfd,ens,1024,100);
		for(int n=0;n<num;n++)
		{
			data* msg=(data*)ens[n].data.ptr;
			msg->call_back(msg);
		}
	}	
}

bool reactor::check_event_exit(int fd,int events)
{
	
}

bool reactor::add_event(int fd,int events,func events_act)
{
	struct data* msg=new data;
	msg->epfd=this->epfd;
	msg->fd=fd;
	msg->call_back=events_act;
	struct epoll_event ev;

	ev.events=events;	
	ev.data.ptr=(void*)msg;
	epoll_ctl(this->epfd,EPOLL_CTL_ADD,fd,&ev);
	//判断是否存在然后再做处理
	
}

bool reactor::del_event(int fd,int events,func events_act)
{
	struct data* msg=new data;
	msg->epfd=this->epfd;
	msg->fd=fd;
	msg->call_back=events_act;
	struct epoll_event ev;

	ev.events=events;	
	ev.data.ptr=(void*)msg;
	epoll_ctl(this->epfd,EPOLL_CTL_DEL,fd,&ev);
	//判断是否存在然后再做处理
}

bool reactor::mod_event(int fd,int events,func events_act)
{
	struct data* msg=new data;
	msg->epfd=this->epfd;
	msg->fd=fd;
	msg->call_back=events_act;
	struct epoll_event ev;

	ev.events=events;	
	ev.data.ptr=(void*)msg;
	epoll_ctl(this->epfd,EPOLL_CTL_MOD,fd,&ev);
	//判断是否存在然后再做处理	
}


int react_in(void* arg)
{
	char buf[1024];
	//接受套接字的读数据  并注册下一步处理函数
	data* msg=(data*)arg;
	int n=read(msg->fd,buf,1024);
	cout<<"read: "<<buf<<endl;
}

int react_accept(void* arg)
{
	struct epoll_event en;
	data* msg=new data;
//	data* msg=(data*)arg;
	int status=0;
again:
	status_fd=accept((data*)arg->fd,&msg->client,&msg->sock_len);
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
	msg->fd=status_fd;
	msg->epfd=(data*)arg->epfd;
	msg->call_back=react_in;
	en.events=EPOLLIN;
	en.data.ptr=msg;
	reactor_list.append(msg);
	epoll_ctl(msg->epfd,EPOLL_CTL_ADD,status_fd,&en);
	return status;
}
