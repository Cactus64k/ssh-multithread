#include "chunks.h"

MACHINE* machine_create_new(const char* host, const char* user, const char* pass)
{
	MACHINE* machine	= malloc(sizeof(MACHINE));
	assert(machine != NULL);

	memset(machine, 0, sizeof(MACHINE));

	machine->host		= malloc(strlen(host) + 1);
	strcpy(machine->host, host);

	machine->user		= malloc(strlen(user) + 1);
	strcpy(machine->user, user);

	machine->status		= MACHINE_AVIABLE;

	if(pass != NULL)
	{
		machine->pass	= malloc(strlen(pass) + 1);
		strcpy(machine->pass, pass);
	}
	else
		machine->pass	= NULL;

	return machine;
}

int machine_free(MACHINE* machine)
{
	free(machine->host);
	free(machine->user);
	free(machine->pass);
	free(machine);

	return EXIT_SUCCESS;
}
