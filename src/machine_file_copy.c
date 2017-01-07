#include "chunks.h"

int machine_file_copy(ssh_session session, const char* local_file_name, const char* remote_file_name)
{
	assert(local_file_name != NULL);
	assert(remote_file_name != NULL);
	assert(session != NULL);

	FILE* f = fopen(local_file_name, "rb");
	if(f != NULL)
	{
		sftp_session sftp_session	= sftp_new(session);
		if(sftp_init(sftp_session) == SSH_OK)
		{
			sftp_file rf = sftp_open(sftp_session, remote_file_name, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);

			char buff[512];

			while(!feof(f))
			{
				size_t r_count = fread(buff, 1, sizeof(buff), f);
				sftp_write(rf, buff, r_count);
			}
			sftp_close(rf);
		}

		fclose(f);
		sftp_free(sftp_session);
		return EXIT_SUCCESS;
	}


	return EXIT_FAILURE;
}
