#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#undef BUFSIZ
#define BUFSIZ 20

void Usage(char *s)
{
	printf("Usage:\n");
	printf("\t%s src_file dst_file\n",s);
	printf("\tsrc_file:The file need to be copy\n");
	printf("\tdst_file:The file need to copy on\n");
}

int main(int argc,char **argv)
{
	int fd_r = -1;
	int fd_w = -1;
	int ret = -1;
	char buf[BUFSIZ];
	if(argc != 3)
	{
		Usage(argv[0]);
		exit(1);
	}
	fd_r = open(argv[1],O_RDONLY);
	if(fd_r < 0)
	{
		perror("open the src_file failed...\n");
		exit(1);
	}

	fd_w = open(argv[2],O_RDWR | O_CREAT | O_EXCL,0666);
	if(fd_w < 0 && EEXIST == errno)
	{
		fd_w = open(argv[2],O_RDWR);
		if(fd_w < 0)
		{
			perror("open the dst_file failed...\n");
			exit(1);
		}
	}
	while(1)
	{
		do
		{
			bzero(buf,BUFSIZ);
			ret = read(fd_r,buf,BUFSIZ-1);
		}while(ret < 0 && EINTR == errno);
		if(ret < 0)
		{
			perror("read failed...\n");
			exit(1);
		}


		do
		{
			ret = write(fd_w,buf,strlen(buf));
		}while(ret < 0 && EINTR == errno);
		if(ret < 0)
		{
			perror("write failed...\n");
			exit(1);
		}
		
		if(ret == 0)
		{
			break;
		}
		fsync(fd_w);
	}

	close(fd_w);
	close(fd_r);

	return 0;
}
