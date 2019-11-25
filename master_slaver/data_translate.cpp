#include "data_translate.h"
#include<sys/epoll.h>
#include<errno.h>

FEN_DEAL* FEN_DEAL::fen_deal=new FEN_DEAL();

void FEN_DEAL::run()
{
	for(int i=0;i<l_channel.size();i++)
	{
		base_message* msg=new base_message(in);
		// l_channel[i]->handle(msg);
		auto item=l_channel.front();
		l_channel.pop_front();
		l_channel.push_back(item);
	}
}

bool FEN_DEAL::channel_add(channel* cha)
{
	bool in=cha->init();
	l_channel.push_back(cha);
 	
	return in;
}

bool FEN_DEAL::protocol_add(protocol* pro)
{
	bool in=pro->init();
	l_protocol.push_back(pro);
	return in;
}

bool FEN_DEAL::dealer_add(dealer* del)
{
	bool in=del->init();
	l_dealer.push_back(del);
	return in;
}


bool FEN_DEAL::channel_del(channel* cha)
{
	bool in;
	in=cha->fini();

	l_channel.remove(cha);
	delete cha;
	return in;
}

bool FEN_DEAL::protocol_del(protocol*pro)
{
	bool in;
	in=pro->fini();
	l_protocol.remove(pro);

	delete pro;
	return in;
}

bool FEN_DEAL::dealer_del(dealer* del)
{
	bool in;
	in=del->fini();
	l_dealer.remove(del);

	delete del;
	return in;
}

bool FEN_DEAL::fini()
{
	while(l_channel.size()!=0)
	{
		auto exp=l_channel.front();
		
		channel_del(exp);
	}
	while(l_dealer.size()!=0)
	{
		auto exp=l_dealer.front();
		dealer_del(exp);
		
	}
	while(l_protocol.size()!=0)
	{
		auto exp=l_protocol.front();
		protocol_del(exp);
	}

	bool stop=false;
	while(!stop)
	{
		if(l_channel.size()==0&&l_protocol.size()==0&&l_dealer.size()==0)
			stop=true;
	}
	// delete l_feng;
}

bool FEN_DEAL::init()
{
	return true;
}

 void FEN_DEAL::FEN_ADD_PROTOCOL(protocol* pro)
{
	FEN_DEAL::fen_deal->protocol_add(pro);
}

 void FEN_DEAL::FEN_ADD_CHANNEL(channel* cha)
{
	FEN_DEAL::fen_deal->channel_add(cha);
}

 void FEN_DEAL::FEN_ADD_DEALER(dealer* del)
{
	FEN_DEAL::fen_deal->dealer_add(del);
}

 void FEN_DEAL::FEN_DEL_PROTOCOL(protocol*pro)
{
	FEN_DEAL::fen_deal->protocol_del(pro);
}

 void FEN_DEAL::FEN_DEL_CHANNEL(channel* cha)
{
	FEN_DEAL::fen_deal->channel_del(cha);
}

 void FEN_DEAL::FEN_DEL_DEALER(dealer*del)
{
	FEN_DEAL::fen_deal->dealer_del(del);
}

 void FEN_DEAL::FEN_INIT()
{
/* 	if(fen_deal==NULL)
	{
		fen_deal=new FEN_DEAL;
	} */
	// this->init();
}

 void FEN_DEAL::FEN_FINI()
{
	FEN_DEAL::fen_deal->fini();
}

 void FEN_DEAL::FEN_RUN()
{
	FEN_DEAL::fen_deal->run();
}

