#include "chunks.h"

JOB* machine_job_create(size_t count, MACHINE* list, const char* script_path, const char* folder_path, int timeout)
{
	JOB* job			= malloc(sizeof(JOB));
	job->threads		= calloc(count, sizeof(pthread_t));
	job->count			= count;
	job->timeout		= timeout;
	job->list			= list;
	job->script_path	= script_path;
	job->folder_path	= folder_path;

	pthread_mutex_init(&(job->mutex), NULL);

	return job;
}


int machine_job_free(JOB* job)
{
	free(job->threads);

	machine_list_free(job->list);

	pthread_mutex_destroy(&(job->mutex));

	free(job);

	return EXIT_SUCCESS;
}
