#include<iostream>
#include<list>
#include<pthread.h>
using namespace std;
typedef void* (*func)(void*);

struct args
{
	//自定义
};
struct task
{
	func func_task;
	args* arg;
};

class pthread_pool
{
	private:
	
	struct pthread_info
	{
		pthread_mutex_t add_task_mutex;
		pthread_cond_t task_cond;
		list<task*> task_list;
		// int num_task=0;
		int pth_num;
		// int current_num_pth;
		// pthread_mutex_t add_pthread;
		list<pthread_t> current_pth;
	};


	public:

	pthread_info* p_info;
	pthread_pool();
	~pthread_pool(){}
	
	int add_task(func function,args* arg);
	void run(pthread_info* info);		//主线程
	static void* task_pthread(void* args);		//工作线程  
						//从task中提取任务放入线程中进行运行

};