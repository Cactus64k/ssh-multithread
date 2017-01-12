#include "chunks.h"

int alloc_snprintf(char** out, const char* template, ...)
{
	va_list list;
	va_start(list, template);

	int len = vsnprintf(NULL, 0, template, list);
	va_end(list);
	char* new_string = malloc((size_t)len + 1);
	assert(new_string != NULL);

	*out = new_string;
	va_start(list, template);
	len = vsnprintf(new_string, (size_t)len+1, template, list);
	va_end(list);

	return len;
}

int machine_hndl(MACHINE* machine, const char* script_path, const char* folder_path, int timeout)
{
	assert(machine != NULL);
	assert(script_path != NULL);

	char* hostname	= machine->host;
	char* user		= machine->user;
	char* pass		= machine->pass;

	ssh_session session	= ssh_new();
	assert(session != NULL);

	ssh_options_set(session, SSH_OPTIONS_HOST, hostname);

	if(ssh_connect(session) == SSH_OK)
	{
		int auth_status = (pass == NULL)?\
			ssh_userauth_autopubkey(session, NULL):\
			ssh_userauth_password(session, user, pass);

		if(auth_status == SSH_AUTH_SUCCESS)
		{
			if(machine_file_copy(session, script_path, REMOTE_FILE_NAME) == EXIT_SUCCESS)
			{
				char* output_file_name = NULL;
				alloc_snprintf(&output_file_name, "%s/%s_output.txt", folder_path, machine->host);

				FILE* output_file = fopen(output_file_name, "wb");
				free(output_file_name);

				if(machine_file_exec(session, REMOTE_COMMAND, output_file, timeout) == EXIT_SUCCESS)
					machine->status = MACHINE_DONE;
				else
					machine->status = MACHINE_SCRIPT_ERROR;

				fflush(output_file);
				fclose(output_file);
			}
			else
				machine->status = MACHINE_UPLOAD_ERROR;
		}
		else
			machine->status = MACHINE_AUTH_ERROR;
	}
	else
		machine->status = MACHINE_CONNECTION_ERROR;

	ssh_disconnect(session);
	ssh_free(session);

	return EXIT_SUCCESS;
}


