#include "chunks.h"

int machine_copy_file(const char* file_name, ssh_session session)
{
	FILE* f							= fopen(file_name, "rb");
	if(f != NULL)
	{
		sftp_session sftp_session	= sftp_new(session);
		if(sftp_init(sftp_session) == 0)
		{
			sftp_file rf			= sftp_open(sftp_session, "/tmp/machine_file", O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);

			char buff[512];

			while(!feof(f))
			{
				size_t r_count = fread(buff, sizeof(buff), 1, f);
				sftp_write(rf, buff, r_count);
			}

			sftp_close(rf);
		}

		fclose(f);
		sftp_free(sftp_session);
	}


	return EXIT_SUCCESS;
}
