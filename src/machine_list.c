#include "chunks.h"

int machine_list(const char* file_name, MACHINE** __list)
{
	int status	= EXIT_SUCCESS;
	FILE* f		= fopen(file_name, "r");
	MACHINE* list	= NULL;
	MACHINE* last	= NULL;

	while(!feof(f))
	{
		char buff[64*3] = {0};
		fgets(buff, sizeof(buff) -1, f);

		size_t name_len = strlen(buff);
		if(name_len > 1)
		{
			char user[64];
			char pass[64];
			char host[64];


			if(sscanf(buff, "%[a-zA-Z0-9]:%[a-zA-Z0-9]@%[a-zA-Z0-9]", user, pass, host) == 3)
			{
				MACHINE* machine	= malloc(sizeof(MACHINE));
				assert(machine != NULL);

				machine->host		= malloc(strlen(host) + 1);
				strcpy(machine->host, host);

				machine->user		= malloc(strlen(user) + 1);
				strcpy(machine->user, user);

				machine->pass		= malloc(strlen(pass) + 1);
				strcpy(machine->pass, pass);

				machine->status		= MACHINE_AVIABLE;
				machine->next		= NULL;
				machine->prev		= last;

				if(list == NULL)
					list		= machine;
				else
					last->next	= machine;

				last			= machine;
			}
			else
				fprintf(stderr, "ERROR: Failed to parsing string %s\n", buff);
		}
	}
	*__list = list;

	return status;
}

int machine_list_free(MACHINE* list)
{
	while(list != NULL)
	{
		char* name		= list->host;
		MACHINE* cur	= list;

		list = list->next;

		free(name);
		free(cur);
	}
	return EXIT_SUCCESS;
}
