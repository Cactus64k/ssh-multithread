#include "chunks.h"

int machine_connect(MACHINE* machine)
{
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
			//machine_copy_file()

			ssh_channel channel	= ssh_channel_new(session);
			assert(channel != NULL);

			ssh_channel_send_eof(channel);
			ssh_channel_close(channel);
			ssh_channel_free(channel);
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


