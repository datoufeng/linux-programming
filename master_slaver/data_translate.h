#include<iostream>
#include<string>
#include<list>
#include<vector>
#include<unistd.h>

using namespace std;

enum IO_DATA{in,out};

struct base_message
{
    base_message(){}
    base_message(IO_DATA io)
	{
        this->io_data=io;
	}
    virtual ~base_message(){}
	IO_DATA io_data;
    string value;
    void* ptr;
};

class base_handle
{
    public:
    base_handle(){}
    virtual ~base_handle(){}
    void handle(base_message* input);
    
    virtual base_message* internel_handle(base_message* input_msg)=0;
    virtual base_handle* get_next_handle(base_message* input_msg)=0;
};

class protocol;

class channel:public base_handle
{
public:
    list<string> data_buf;
	
    bool need_close=false;
	protocol* f_protocol;
	
	bool fini_flag=false;
	
public:
    channel()
	{
		// init();
	}
    virtual ~channel()
	{
		// fini();
		// f_protocol->channel_exit=false;
	}
	
    virtual bool init()=0;

    virtual bool fini()=0;
	
/*     bool get_close_statement()
	{
        return need_close;
    }
	
    void set_close(bool state)
	{
        this->need_close=state;
    }

    bool if_empty()
	{
        return true==data_buf.empty();
    } */
    void bind_protocol(protocol* i_protocol)
	{
        this->f_protocol=i_protocol;
    }
	
public:
    virtual base_message* in_message_handle(base_message* msg)=0;
    virtual base_message* out_message_handle(base_message* msg)=0;
	
public:
    virtual base_message* internel_handle(base_message* input_msg);
    virtual base_handle* get_next_handle(base_message* input_msg);
};

class dealer;

class protocol:public base_handle
{
public:
    protocol()
	{
		// init();
	}
    virtual ~protocol()
	{
		// fini();
	}
	bool channel_exit=false;
	bool dealer_exit=false;
	
	channel* f_channel=NULL;
	dealer* f_dealer=NULL;
	
	bool fini_flag=false;
public:
    virtual base_message* raw_to_struct(base_message* msg)=0;
    virtual base_message* struct_to_raw(base_message* msg)=0;
    
public:
	virtual bool init()=0;
	bool fini()
	{
		int i=0;
		fini_flag=true;
		while(true)
		{
			if(this->channel_exit==true&&this->dealer_exit==true)
			{
				// delete this;
				return true;
			}
			else
			{
				i++;
				if(i>10)
					break;
				sleep(1);
			}
		}
		return false;
	}
	
    virtual void bind_channel(channel* i_channel)
	{
        this->f_channel=i_channel;
    }
    virtual void bind_dealer(dealer* i_dealer)
	{
        this->f_dealer=i_dealer;
    }
    virtual base_message* internel_handle(base_message* input_msg);
    virtual base_handle* get_next_handle(base_message* input_msg);
};

class dealer:base_handle
{
public:
	protocol* f_protocol;
	
public:
	bool fini_flag=false;
    dealer()
	{
		// init();
	}
    virtual ~dealer()
	{
		f_protocol->channel_exit=true;
		// fini();
	}
    virtual bool init()=0;
    virtual bool fini()=0;
    virtual bool bind_protocol(protocol* i_protocol)
	{
        this->f_protocol=i_protocol;
    }
    virtual base_message* msg_handle(base_message* msg)=0;
public:
    virtual base_message* internel_handle(base_message* input_msg);
    virtual base_handle* get_next_handle(base_message* input_msg);
};

class FEN_DEAL
{
    private:
    FEN_DEAL()
	{
        // epfd=epoll_create(1);
    }
    ~FEN_DEAL()
	{
        // close(epfd);
    }
    
    int epfd=-1;
    // static FEN_DEAL* l_feng;
    list<channel*> l_channel;
    list<protocol*> l_protocol;
    list<dealer*> l_dealer;
	
    bool channel_add(channel* cha);
    bool protocol_add(protocol* pro);
    bool dealer_add(dealer* del);
    
    bool channel_del(channel* cha);
    bool protocol_del(protocol*pro);
    bool dealer_del(dealer* del);
	bool fini();
	bool init();
	void run();
	bool manage();
	
    public:
	static void FEN_ADD_PROTOCOL(protocol* pro);
    static void FEN_ADD_CHANNEL(channel* cha);
    static void FEN_ADD_DEALER(dealer* del);
    static void FEN_DEL_PROTOCOL(protocol*pro);
    static void FEN_DEL_CHANNEL(channel* cha);
    static void FEN_DEL_DEALER(dealer*del);
    static void FEN_INIT();
    static void FEN_FINI();
	
    static void FEN_RUN();

	static FEN_DEAL* fen_deal;
	
};

