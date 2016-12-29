#include "chunks.h"

JOB* job__ = NULL;

static void* worker(void* ptr)
{
	//JOB* job				= (JOB*)ptr;
	JOB* job				= job__;
	MACHINE* list			= job->list;
	pthread_mutex_t* mutex	= &(job->mutex);
	for(MACHINE* item=list; item != NULL; item=item->next)
	{
		if(pthread_mutex_lock(mutex) == 0)
		{
			if(item->status == MACHINE_AVIABLE)
			{
				item->status = MACHINE_EXECUTING;
				pthread_mutex_unlock(mutex);
				printf("%d: executing machine %s\n", (int)ptr, item->host);
				sleep(1);
				item->status = MACHINE_DONE;
			}
		}

		pthread_mutex_unlock(mutex);
	}

	//TODO offline machines

	return NULL;
}

JOB* machine_create_job(size_t count, MACHINE* list, const char* script_path)
{
	JOB* job			= malloc(sizeof(JOB));
	job->threads		= calloc(count, sizeof(pthread_t));
	job->count			= count;
	job->list			= list;
	job->script_path	= script_path;

	pthread_mutex_init(&(job->mutex), NULL);

	return job;
}


int machine_start_job(JOB* job)
{
	job__ = job;

	size_t count = job->count;

	for(size_t i=0; i<count; i++)
	{
		int res = pthread_create((job->threads) + i, NULL, worker, i);
		assert(res == 0);
	}

	pthread_join(job->threads[0], NULL);

	return EXIT_SUCCESS;
}
