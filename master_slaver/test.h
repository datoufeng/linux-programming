#include"data_translate.h"
#include"../common/tcp.h"
#include"../common/common.h"
#include<string>
#include<list>
#include<unistd.h>
#include<fcntl.h>
#include<sys/epoll.h>
struct test_struct
{
	string id;
	string name;
	string address;
};
class t_channel:public channel
{
	public:
	int fd=-1;
	// int epfd=-1;
	
	t_channel(){}
	~t_channel(){}
	virtual bool init()
	{
		this->fd=open("./test.txt",O_RDONLY);
		cout<<"fd: "<<this->fd<<endl;
		// this->epfd=epoll_create(1);
	}
	virtual bool fini()
	{
		close(fd);
		// close(lis_fd);
	}
	
    virtual base_message* in_message_handle(base_message* msg)
	{
		char ptr[1024];
		int n=1;
		msg->io_data=in;
		while(n!=0&&n>0)
		{
			n=read(fd,ptr,1024);
			// string str=ptr;
			// data_buf.append(str);
			
			msg->value=ptr;
			cout<<"readline:"<<ptr;
			msg->ptr=new test_struct();
		}

	}
    virtual base_message* out_message_handle(base_message* msg)
	{
		return NULL;
	}

};

class t_protocol:public protocol
{
	public:
    virtual base_message* raw_to_struct(base_message* msg)
	{
		int n1=0,n2=0;
		n2=msg->value.find(" ",n1);
		((test_struct*)(msg)->ptr)->id=(msg->value).substr(n1,n2);
		cout<<"raw_to_struct id: "<<((test_struct*)(msg)->ptr)->id<<endl;
		n1=n2;
		n2=msg->value.find(" ",n1);
		((test_struct*)(msg)->ptr)->name=(msg->value).substr(n1,n2);
		cout<<"raw_to_struct name: "<<((test_struct*)(msg)->ptr)->id<<endl;

		n1=n2;
		n2=msg->value.find(" ",n1);
		((test_struct*)(msg)->ptr)->address=(msg->value).substr(n1,n2);
		cout<<"raw_to_struct address: "<<((test_struct*)(msg)->ptr)->id<<endl;

		return msg;
	}
	
    virtual base_message* struct_to_raw(base_message* msg)
	{
		return NULL;
	}
	bool init()
	{
		return true;
	}
};

class t_dealer:public dealer
{
	public:
    virtual bool init()
	{
		return true;
	}
	
    virtual bool fini()
	{
		return true;
	}

    virtual base_message* msg_handle(base_message* msg)
	{
		cout<<"***************************"<<endl;
		cout<<"id:"<<((test_struct*)(msg)->ptr)->id<<endl;
		cout<<"name:"<<((test_struct*)(msg)->ptr)->name<<endl;
		cout<<"address:"<<((test_struct*)(msg)->ptr)->address<<endl;
		return NULL;
	}

};

