#include "chunks.h"

int exec_proc(ssh_channel channel, const char* command, FILE* f)
{
	int status = EXIT_SUCCESS;
	if(ssh_channel_open_session(channel) == SSH_OK)
	{
		char buff[512];
		int r_count = 0;
		int result = ssh_channel_request_exec(channel, command);

		if(f != NULL)
		{
			do
			{
				r_count = ssh_channel_read(channel, buff, sizeof(buff), 0);
				fwrite(buff, 1, r_count, f);
			}
			while(r_count > 0);

			if(r_count != 0)
			{
				status = EXIT_FAILURE;
			}
		}
	}
	else
	{
		status = EXIT_FAILURE;
		fprintf(stderr, "ERROR: Failed to create channel. %s\n", ssh_get_error(channel));
	}



	return status;
}
