#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
void Usage(char *s)
{
	printf("Usage:\n");
	printf("\t%s dst_file\n",s);
	printf("\tdts_file:The file need to create\n");
}

int main(int argc,char **argv)
{
	int fd;
	if(argc != 2)
	{
		Usage(argv[0]);
		exit(1);
	}
	fd = open(argv[1],O_RDWR | O_CREAT,0666);
	if(fd < 0)
	{
		fprintf(stderr,"open %s failed....\n",argv[1]);
		exit(1);
	}
	close(fd);

	return 0;
}
