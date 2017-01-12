#ifndef CHUNKS_H_
	#define CHUNKS_H_

	#include <stdlib.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <string.h>
	#include <assert.h>
	#include <stdbool.h>
	#include <stdarg.h>

	#include <argp.h>
	#include <pthread.h>
	#include <libssh/libssh.h>
	#include <libssh/sftp.h>
	#include <unistd.h>
	#include <sys/stat.h>
	#include <fcntl.h>

	typedef struct MACHINE
	{
		char* host;
		char* user;
		char* pass;
		enum
		{
			MACHINE_AVIABLE,
			MACHINE_EXECUTING,
			MACHINE_DONE,

			MACHINE_CONNECTION_ERROR,
			MACHINE_SCRIPT_ERROR,
			MACHINE_AUTH_ERROR,
			MACHINE_UPLOAD_ERROR,
		} status;

		struct MACHINE* next;
		struct MACHINE* prev;
	} MACHINE;

	typedef struct JOB
	{
		pthread_mutex_t mutex;
		pthread_t* threads;
		size_t count;
		int timeout;
		MACHINE* list;
		const char* script_path;
		const char* folder_path;
	} JOB;

	#define REMOTE_COMMAND "bash -c /tmp/ssh-multithread.script"
	#define REMOTE_FILE_NAME "/tmp/ssh-multithread.script"

	int machine_list(const char* file_name, MACHINE** __list);
	int machine_list_free(MACHINE* list);
	int machine_free(MACHINE* machine);
	MACHINE* machine_create_new(const char* host, const char* user, const char* pass);

	JOB* machine_job_create(size_t count, MACHINE* list, const char* script_path, const char* folder_path, int timeout);
	int machine_job_free(JOB* job);
	int machine_start_job(JOB* job);

	int machine_hndl(MACHINE* machine, const char* script_path, const char* output_folder, int timeout);
	int machine_file_copy(ssh_session session, const char* local_file_name, const char* remote_file_name);
	int machine_file_exec(ssh_session session, const char* command, FILE* f, int timeout);
	int machine_print_status(MACHINE* machine);

#endif /* CHUNKS_H_ */
