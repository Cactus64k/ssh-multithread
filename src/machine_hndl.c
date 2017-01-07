#include "chunks.h"

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
		if(ssh_userauth_password(session, user, pass) == SSH_OK)
		{
			const char* remote_file_name = "/tmp/ssh-multithread.script";
			if(machine_file_copy(session, script_path, remote_file_name) == EXIT_SUCCESS)
			{
				const char* command = "bash -c /tmp/ssh-multithread.script";

				int len = snprintf(NULL, 0, "%s/%s_output.txt", folder_path, machine->host);
				char* output_file_name = malloc((size_t)len + 2);
				snprintf(output_file_name, (size_t)len+1, "%s/%s_output.txt", folder_path, machine->host);

				FILE* output_file = fopen(output_file_name, "wb");
				free(output_file_name);

				if(machine_file_exec(session, command, output_file, timeout) == EXIT_SUCCESS)
					machine->status = MACHINE_DONE;
				else
					machine->status = MACHINE_SCRIPT_ERROR;

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


