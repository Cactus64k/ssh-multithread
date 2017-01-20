#include "chunks.h"

static void* worker(void* ptr)
{
	JOB* job					= (JOB*)ptr;
	MACHINE* list				= job->list;
	int timeout					= job->timeout;
	pthread_mutex_t* mutex		= &(job->mutex);
	const char* script_path		= job->script_path;
	const char* folder_path		= job->folder_path;

	for(MACHINE* item=list; item!=NULL; item=item->next)
	{
		if(pthread_mutex_lock(mutex) == 0)
		{
			if(item->status == MACHINE_AVIABLE)
			{
				item->status = MACHINE_EXECUTING;
				pthread_mutex_unlock(mutex);

				printf("THREAD: Handle machine %s\n", item->host);
				machine_hndl(item, script_path, folder_path, timeout);
			}
		}

		pthread_mutex_unlock(mutex);
	}

	return NULL;
}


int machine_start_job(JOB* job)
{
	size_t count = job->count;

	for(size_t i=0; i<count; i++)
	{
		int res = pthread_create((job->threads) + i, NULL, worker, job);
		assert(res == 0);
	}

	for(size_t i=0; i<count; i++)
		pthread_join(job->threads[i], NULL);

	fprintf(stdout, "THREAD: All threads done\n");
	fflush(stdout);

	MACHINE* list = job->list;

	puts("######STATUS OF MACHINES######\n");
	for(MACHINE* item=list; item!=NULL; item=item->next)
		machine_print_status(item);
	puts("##############################\n");

	if(job->wait_offline == true)
	{
		puts("THREAD: Handle offline machines in one thread.\n");

		bool offline_mashines		= false;
		int timeout					= job->timeout;
		const char* script_path		= job->script_path;
		const char* folder_path		= job->folder_path;
		do
		{
			offline_mashines		= false;
			for(MACHINE* item=list; item!=NULL; item=item->next)
			{
				if(item->status == MACHINE_CONNECTION_ERROR)
				{
					item->status = MACHINE_EXECUTING;
					machine_hndl(item, script_path, folder_path, timeout);
					machine_print_status(item);
					if(item->status == MACHINE_CONNECTION_ERROR)
						offline_mashines = true;
				}
			}
		}
		while(offline_mashines == true);
	}

	return EXIT_SUCCESS;
}
