#include "data_translate.h"
void base_handle::handle(base_message* input)
{
    base_message* next_msg=internel_handle(input);
	if(next_msg!=NULL)
	{
		base_handle* next_handle=get_next_handle(next_msg);
		if(next_handle!=NULL)
		{
			next_handle->handle(next_msg);
		}
	}
}

base_message* channel::internel_handle(base_message* input_msg)
{
	cout<<"io_data in: "<<in<<endl;
	cout<<"io_data out: "<<out<<endl;

	if(input_msg->io_data==in)
		input_msg=this->in_message_handle(input_msg);
	else if(input_msg->io_data==out)
		input_msg=this->out_message_handle(input_msg);
	else
		return NULL;
	return input_msg;
}

base_handle* channel::get_next_handle(base_message* input_msg)
{
	if(input_msg->io_data==in)
	{
		return this->f_protocol;
	}
	else if(input_msg->io_data==out)
	{
		data_buf.push_back(input_msg->value);
		return NULL;
	}
	else
		return NULL;
}

base_message* protocol::internel_handle(base_message* input_msg)
{
	if(input_msg->io_data==in)
	{
		return this->raw_to_struct(input_msg);
	}
	else if(input_msg->io_data==out)
	{
		return this->struct_to_raw(input_msg);
	}
	else
		return NULL;
}

base_handle* protocol::get_next_handle(base_message* input_msg)
{
	// return this->raw_to_struct(input_msg);	//message结构体判断是否进行下一步
	if(input_msg->io_data==in)
	{
		return (base_handle*)this->f_dealer;
	}
	else if(input_msg->io_data==out)
	{
		return this->f_channel;
	}
	else
		return NULL;
}

base_message* dealer::internel_handle(base_message* input_msg)
{
	return this->msg_handle(input_msg);//通过标志位，判断是否进行下一步
}

base_handle* dealer::get_next_handle(base_message* input_msg)
{
	return this->f_protocol;//通过标志位判断，下一步是给dealer ， 还是protocol
}

