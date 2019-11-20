#include<iostream>
#include<list>
#include<pthread.h>
using namespace std;
typedef void* (*func)(void*);	//自定义函数

struct args		//自定义参数
{
	//自定义
};

struct task
{
	int urgency;
	func func_task;
	args* arg;
};
class compare_task
{
	public:
	bool operator()(const task* s1,const task* s2)
	{
		return s1->urgency > s2->urgency;
	}
};
class pthread_pool
{
	private:
	
	struct pthread_info
	{
		pthread_mutex_t add_task_mutex;
		
		pthread_mutex_t add_pthread_mutex;
		
		pthread_cond_t task_cond;
		list<task*> task_list;
		int task_max_to_add_path;	//task达到一定数量后开始增加线程
		int pth_num;		//开始线程开启数量
		int pth_extent;		//线程增加数量
		list<pthread_t> current_pth;
	};

	public:

	pthread_info* p_info;
	pthread_pool(int pth_num=4,int pth_extent=2,int task_max_to_add_path=20);
	~pthread_pool(){}
	
	int add_task(func function,args* arg,int urge=0);
	void run();		//主线程
	static void* task_pthread(void* args);		//工作线程  
						//从task中提取任务放入线程中进行运行
	void* manage_pthread();
	static void* task_pthread_tmp(void* args);
};