#include "chunks.h"

int machine_print_status(MACHINE* machine)
{
	switch (machine->status)
	{
		case MACHINE_UPLOAD_ERROR:
		{
			fprintf(stderr, "ERROR: Failed to upload file %s\n", machine->host);
			break;
		}
		case MACHINE_AUTH_ERROR:
		{
			fprintf(stderr, "ERROR: Failed to authentificate %s\n", machine->host);
			break;
		}
		case MACHINE_CONNECTION_ERROR:
		{
			fprintf(stderr, "ERROR: Failed to connect %s\n", machine->host);
			break;
		}
		case MACHINE_SCRIPT_ERROR:
		{
			fprintf(stderr, "ERROR: Failed to exec script %s\n", machine->host);
			break;
		}
		case MACHINE_DONE:
		{
			fprintf(stdout, "SUCSESS: %s\n", machine->host);
			break;
		}
		case MACHINE_AVIABLE:
		{
			fprintf(stderr, "AVIABLE: %s\n", machine->host);
			break;
		}
		case MACHINE_EXECUTING:
		{
			fprintf(stderr, "EXECUTING: %s\n", machine->host);
			break;
		}
	}

	return EXIT_SUCCESS;
}
