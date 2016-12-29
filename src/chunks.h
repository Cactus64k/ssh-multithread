/*
 * chunks.h
 *
 *  Created on: 18.12.2016
 *      Author: cactus
 */

#ifndef CHUNKS_H_
	#define CHUNKS_H_

	#include <stdlib.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <string.h>
	#include <assert.h>

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
		} status;

		struct MACHINE* next;
		struct MACHINE* prev;
	} MACHINE;

	typedef struct JOB
	{
		pthread_mutex_t mutex;
		pthread_t* threads;
		size_t count;
		MACHINE* list;
		const char* script_path;
	} JOB;

	int machine_list(const char* file_name, MACHINE** __list);
	int machine_list_free(MACHINE* list);

	JOB* machine_create_job(size_t count, MACHINE* list, const char* script_path);
	int machine_start_job(JOB* job);

	int machine_connect(MACHINE* machine);

#endif /* CHUNKS_H_ */
