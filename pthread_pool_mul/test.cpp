#include"pthread_pool.h"
#include<string>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
using namespace std;
struct data:public args
{
	string name;
	string address;
	int id;
};
void* print(void* args)
{
	data* da=(data*)args;
	char buf[10]="zhang";
	cout<<"name:"<<da->name<<endl;
	int fd=open("a.txt",O_WRONLY);
	write(fd,buf,10);
	cout<<"address:"<<da->address<<endl;
	
}
int main(int argc,char** argv)
{
	pthread_pool pool;
	data* a=new data;
	a->name="zhang";
	a->address="anhui-luan";
	for(int i=0;i<30;i++)
		pool.add_task(print,a);
	pool.run();
	pthread_exit(NULL);
}