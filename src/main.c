#include "chunks.h"

static int parse_opt(int key, char* arg, struct argp_state* state);
char* machines_list_path	= NULL;
char* script_path			= NULL;
char* folder_path			= NULL;
int threads_count			= 2;
int timeout					= 3000;
bool wait_offline			= true;

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	static struct argp_option options[] = {
	{"script" ,				's',	"path",	0,	"Script to exec on remote machine",	0},
	{"threads" ,			'c',	"2",	0,	"Count of using threads",			0},
	{"machines" ,			'l',	"file",	0,	"File with machines list",			0},
	{"timeout" ,			't',	"2",	0,	"Timeout execution in seconds",		0},
	{"output_folder" ,		'f',	"path",	0,	"Folder for output files",			0},
	{"wait_offline" ,		'o',	"true",	0,	"Handle offline machines",			0},
	{0}};

	struct argp argp = {0};
	argp.options	= options;
	argp.parser		= parse_opt;
	argp.doc		=	"Machine list format:"
						"\n\nlogin:password@hostname\n"
						"login2:password2@hostname2\n\n"
						"Or login@hostname if you have public key authorisation. Each machine on a new line. Offline machines handled in main thread.";
	//argp.args_doc	= "--arg=value";

	argp_parse (&argp, argc, argv, 0, 0, NULL);

	if(machines_list_path == NULL || script_path == NULL)
	{
		fprintf(stderr, "\nERROR: --script and --machines should have arguments\n");
		return EXIT_FAILURE;
	}
	else
	{
		ssh_init();
		MACHINE* list = NULL;
		machine_list(machines_list_path, &list);

		JOB* job = machine_job_create((size_t)threads_count, list, script_path, folder_path, timeout, wait_offline);
		machine_start_job(job);

		printf("THREAD: freeing memory\n");
		machine_job_free(job);

		free(machines_list_path);
		free(script_path);
		free(folder_path);

		ssh_finalize();
	}

	return EXIT_SUCCESS;
}

static int parse_opt (int key, char* arg, struct argp_state* state)
{
	switch (key)
	{
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
				fprintf(stderr, "\nERROR: Failed to open file %s\n", arg);
				exit(EXIT_FAILURE);
			}
			break;
		}
		case 'c':
		{
			if(sscanf(arg, "%d", &threads_count) != 1)
			{
				fprintf(stderr, "\nERROR: Failed to parse threads count %s\n", arg);
				exit(EXIT_FAILURE);
			}

			if(threads_count > 64)
			{
				fprintf(stderr, "\nERROR: Too many threads. The maximum number of threads is 64\n");
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
				fprintf(stderr, "\nERROR: Failed to open file %s\n", arg);
				exit(EXIT_FAILURE);
			}
			break;
		}
		case 't':
		{
			if(sscanf(arg, "%d", &timeout) != 1)
			{
				fprintf(stderr, "\nERROR: Failed to parse timeout %s\n", arg);
				exit(EXIT_FAILURE);
			}
			else
				timeout = timeout * 1000;

			break;
		}
		case 'f':
		{
			struct stat s;
			if(stat(arg, &s) == -1)
			{
				fprintf(stderr, "\nERROR: Folder %s do not exist\n", arg);
				exit(EXIT_FAILURE);
			}
			else
			{
				if(S_ISDIR(s.st_mode))
				{
					size_t arg_len = strlen(arg);
					folder_path = malloc(arg_len+1);
					memcpy(folder_path, arg, arg_len+1);
				} else
				{
					fprintf(stderr, "\nERROR: %s is not a folder\n", arg);
					exit(EXIT_FAILURE);
				}
			}

			break;
		}
		case 'o':
		{
			if(strcmp("false", arg) == 0)
				wait_offline = false;
			else if(strcmp("true", arg) == 0)
				wait_offline = true;
			else
			{
				fprintf(stderr, "\nERROR: unknown paramether in option --wait_offline=%s\n", arg);
				exit(EXIT_FAILURE);
			}

			break;
		}
	}
	return 0;
}

