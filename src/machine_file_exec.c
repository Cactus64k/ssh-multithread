#include "chunks.h"

int machine_file_exec(ssh_session session, const char* command, FILE* f, int timeout)
{
	assert(command != NULL);
	assert(f != NULL);

	ssh_channel channel	= ssh_channel_new(session);
	assert(channel != NULL);

	int status = EXIT_SUCCESS;
	if(ssh_channel_open_session(channel) == SSH_OK)
	{
		if(ssh_channel_request_exec(channel, command) == SSH_ERROR)
			status	= EXIT_FAILURE;

		int r_count = 0;
		char buff[512];
		fputs("\n################### STDOUT ###################\n", f);
		do
		{
			r_count = ssh_channel_read_timeout(channel, buff, sizeof(buff), 0, timeout);
			if(r_count != SSH_ERROR)
				fwrite(buff, 1, (size_t)r_count, f);
		}
		while(r_count > 0);

		fputs("\n################### STDERR ###################\n", f);

		do
		{
			r_count = ssh_channel_read_timeout(channel, buff, sizeof(buff), 1, timeout);
			if(r_count != SSH_ERROR)
				fwrite(buff, 1, (size_t)r_count, f);
		}
		while(r_count > 0);
	}
	else
	{
		status = EXIT_FAILURE;
		fprintf(stderr, "\nERROR: Failed to create channel. %s\n", ssh_get_error(channel));
	}

	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);

	return status;
}
