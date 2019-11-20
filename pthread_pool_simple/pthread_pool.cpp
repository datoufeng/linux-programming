#include"pthread_pool.h"
pthread_pool::pthread_pool()
{
	p_info=new pthread_info();
	pthread_mutex_init(&(p_info->add_task_mutex),NULL);
	pthread_cond_init(&(p_info->task_cond),NULL);
	p_info->pth_num=4;
}
int pthread_pool::add_task(func function,args* arg)
{
	task* tsk=new task;
	tsk->func_task=function;
	tsk->arg=arg;
	pthread_mutex_lock(&(p_info->add_task_mutex));
	p_info->task_list.push_back(tsk);
	// this->info->num_task++;
	pthread_mutex_unlock(&(p_info->add_task_mutex));
	pthread_cond_signal(&(p_info->task_cond));
}

void pthread_pool::run(pthread_info* info)
{
	for(int i=0;i<p_info->pth_num;i++)
	{
		pthread_t p_id;
		pthread_create(&p_id,NULL,task_pthread,(void*)(this->p_info));
		p_info->current_pth.push_back(p_id);
	}
}

void* pthread_pool::task_pthread(void* args)
{
	pthread_info* info=(pthread_info*)args;
	while(true)
	{
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
		
	}
}

