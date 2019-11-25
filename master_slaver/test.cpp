#include<iostream>
// #include"data_translate.h"
#include"test.h"
using namespace std;
int main(int argc,char** argv)
{
	t_channel a;
	t_protocol b;
	t_dealer c;
	a.bind_protocol(&b);
	b.bind_channel(&a);
	b.bind_dealer(&c);
	c.bind_protocol(&b);
	a.init();
	base_message msg;
	msg.io_data=in;
	a.handle(&msg);
}

