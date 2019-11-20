#include"pthread_pool.h"
#include<unistd.h>
#include<algorithm>
#include<list>
pthread_pool::pthread_pool(int pth_num,int pth_extent,int task_max_to_add_path)
{
	p_info=new pthread_info();
	pthread_mutex_init(&(p_info->add_task_mutex),NULL);
	pthread_cond_init(&(p_info->task_cond),NULL);
	p_info->pth_num=pth_num;
	p_info->pth_extent=pth_extent;
	p_info->task_max_to_add_path=task_max_to_add_path;
	
}

int pthread_pool::add_task(func function,args* arg,int urge)
{
	task* tsk=new task;
	tsk->func_task=function;
	tsk->arg=arg;
	tsk->urgency=urge;
	pthread_mutex_lock(&(p_info->add_task_mutex));
	p_info->task_list.push_back(tsk);
	p_info->task_list.sort(compare_task());
	pthread_mutex_unlock(&(p_info->add_task_mutex));
	pthread_cond_signal(&(p_info->task_cond));
}

void pthread_pool::run()
{
	for(int i=0;i<p_info->pth_num;i++)
	{
		pthread_t p_id;
		pthread_create(&p_id,NULL,task_pthread,(void*)(this->p_info));
	}
	manage_pthread();
}

void* pthread_pool::task_pthread(void* args)
{
	pthread_info* info=(pthread_info*)args;
	while(true)
	{
		pthread_mutex_lock(&(info->add_pthread_mutex));
		info->current_pth.push_back(pthread_self());
		pthread_mutex_unlock(&(info->add_pthread_mutex));
		
		pthread_mutex_lock(&(info->add_task_mutex));
		while(info->task_list.size()==0)
		{
			pthread_cond_wait(&(info->task_cond),&(info->add_task_mutex));		
		}
		task* tasks=info->task_list.front();
		info->task_list.pop_front();
		void* ex=(void*)tasks->arg;
		pthread_mutex_unlock(&(info->add_task_mutex));
		tasks->func_task(ex);		/* reinterpret_cast<void*>(tasks->arg) */
		// sleep(1);
	}
}

void* pthread_pool::manage_pthread()
{
	while(true)
	{
		if((p_info->task_list.size() >= p_info->task_max_to_add_path)&&\
				p_info->current_pth.size() <= (p_info->pth_num+p_info->pth_extent))
		{
			pthread_t p_id;
			pthread_create(&p_id,NULL,task_pthread_tmp,(void*)(this->p_info));
			
		}
		// sleep(0.5);
		
	}
	
}

void* pthread_pool::task_pthread_tmp(void* args)
{
	pthread_info* info=(pthread_info*)args;
	while(true)
	{
		pthread_mutex_lock(&(info->add_pthread_mutex));
		info->current_pth.push_back(pthread_self());
		pthread_mutex_unlock(&(info->add_pthread_mutex));
		
		pthread_mutex_lock(&(info->add_task_mutex));
/* 		while(info->task_list.size()==0)
		{
			pthread_cond_wait(&(info->task_cond),&(info->add_task_mutex));			
		}
 */		

		task* tasks=info->task_list.front();
		info->task_list.pop_front();
		pthread_mutex_unlock(&(info->add_task_mutex));
		void* ex=(void*)tasks->arg;
		tasks->func_task(ex);		/* reinterpret_cast<void*>(tasks->arg) */
		if(info->task_list.size() < (info->task_max_to_add_path)/2)
		{
			// cout<<"break++++++++++++++"<<endl;
			break;
		}
		// cout<<"tmp add"<<endl;
	}
}

