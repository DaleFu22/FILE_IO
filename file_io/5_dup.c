#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(void)
{
	int fd = -1;
	int newfd_1 = -1;
	int newfd_2 = -1;
	fd = open("./dup.txt",O_RDWR | O_CREAT | O_EXCL,0666);
	if(fd < 0 && EEXIST == errno)
	{
		fd = open("./dup.txt",O_RDWR);
		if(fd < 0)
		{
			perror("open the file failed...\n");
			exit(1);
		}
	}
	newfd_1 = dup(fd);
	printf("fd = %d\nnewfd_1 = %d\n",fd,newfd_1);
	write(fd,"hello",strlen("hello"));
	write(newfd_1,"world",strlen("world"));

	newfd_2 = dup2(fd,7);
	printf("newfd_2 = %d\n",newfd_2);
	write(fd,"你好",strlen("你好"));
	write(newfd_2,"世界",strlen("世界"));
		
	close(fd);
	close(newfd_1);
	close(newfd_2);
	return 0;
}
