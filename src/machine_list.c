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
				MACHINE* machine	= machine_create_new(host, user, pass);
				machine->prev		= last;

				if(list == NULL)
					list		= machine;
				else
					last->next	= machine;

				last			= machine;
			}
			else if(sscanf(buff, "%[a-zA-Z0-9]@%[a-zA-Z0-9]", user, host) == 2)
			{
				MACHINE* machine	= machine_create_new(host, user, NULL);
				machine->prev		= last;

				if(list == NULL)
					list		= machine;
				else
					last->next	= machine;

				last			= machine;
			}
			else
				fprintf(stderr, "\nERROR: Failed to parsing string %s\n", buff);
		}
	}
	*__list = list;

	fclose(f);

	return status;
}

int machine_list_free(MACHINE* list)
{
	while(list != NULL)
	{
		MACHINE* cur	= list;

		list = list->next;

		machine_free(cur);
	}
	return EXIT_SUCCESS;
}
