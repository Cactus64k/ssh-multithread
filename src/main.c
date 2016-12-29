#include "chunks.h"

static int parse_opt(int key, char* arg, struct argp_state* state);
char* machines_list_path	= NULL;
char* script_path			= NULL;
int threads_count			= 2;
int timeout					= 2;

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	static struct argp_option options[] = {
	{"script" ,				's',	"file",	0,	"Script to exec on remote machine"},
	{"threads" ,			'c',	"2",	0,	"Count of using threads"},
	{"machines" ,			'l',	"file",	0,	"File with machines list"},
	{"timeout" ,			't',	"2",	0,	"Timeout execution in seconds"},
	{0}
	};

	struct argp argp = { options, parse_opt };

	argp_parse (&argp, argc, argv, 0, 0, NULL);

	if(machines_list_path == NULL || script_path == NULL)
	{
		fprintf(stderr, "ERROR: --script and --machiles should have arguments\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		MACHINE* list = NULL;
		machine_list(machines_list_path, &list);

		machine_connect(list);

		JOB* job = machine_create_job(threads_count, list, NULL);
		machine_start_job(job);

		machine_list_free(list);

		free(machines_list_path);
		free(script_path);
	}

	return EXIT_SUCCESS;
}

static int parse_opt (int key, char* arg, struct argp_state* state)
{
	switch (key)
	{
		case 'c':
		{
			if(sscanf(arg, "%d", &threads_count) != 1)
			{
				fprintf(stderr, "ERROR: Failed to parse threads count %s\n", arg);
				exit(EXIT_FAILURE);
			}

			if(threads_count > 64)
			{
				fprintf(stderr, "ERROR: Too many threads. The maximum number of threads is 64\n");
				exit(EXIT_FAILURE);
			}

			break;
		}
		case 'l':
		{
			FILE* f = fopen(arg, "r");
			if(f != NULL)
			{
				size_t arg_len = strlen(arg);
				machines_list_path = malloc(arg_len+1);
				memcpy(machines_list_path, arg, arg_len+1);

				fclose(f);
			}
			else
			{
				fprintf(stderr, "ERROR: Failed to open file %s\n", arg);
				exit(EXIT_FAILURE);
			}
			break;
		}
		case 't':
		{
			if(sscanf(arg, "%d", &timeout) != 1)
			{
				fprintf(stderr, "ERROR: Failed to parse timeout %s\n", arg);
				exit(EXIT_FAILURE);
			}

			break;
		}
		case 's':
		{
			FILE* f = fopen(arg, "r");
			if(f != NULL)
			{
				size_t arg_len = strlen(arg);
				script_path = malloc(arg_len+1);
				memcpy(script_path, arg, arg_len+1);

				fclose(f);
			}
			else
			{
				fprintf(stderr, "ERROR: Failed to open file %s\n", arg);
				exit(EXIT_FAILURE);
			}
			break;
		}
	}
	return 0;
}

