#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 50
#define QUIT "quit"

void Usage(char *s)
{
	printf("Usage:\n");
	printf("\t%s dst_file\n",s);
	printf("\tdst_file:The file to write in\n");
}

int main(int argc,char **argv)
{
	int fd = -1;
	int ret = -1;
	char buf[SIZE];
	if(argc != 2)
	{
		Usage(argv[0]);
		exit(1);
	}

	fd = open(argv[1],O_RDWR | O_CREAT | O_EXCL,0666);
	if(fd < 0 && EEXIST == errno)
	{
		fd = open(argv[1],O_RDWR);
		if(fd < 0)
		{
			printf("open the dst_file failed...\n");
			exit(1);
		}
	}
	while(1)
	{
		bzero(buf,SIZE);
		printf("Please iniput: \n");
		//小细节：这里要加换行符，不然"Please input"会一直在缓冲区里打印不出来,除非使用无缓冲格式。
		//fprintf(stderr,"Please input: ");

		do{		
			ret = read(0,buf,SIZE-1);
		}while(ret < 0 && EINTR == errno);
		if(ret < 0)
		{
			perror("read failed\n");
			continue;
		}

		if(!strncasecmp(QUIT,buf,strlen(QUIT)))
		{
			printf("quiting...\n");
			break;
		}
		
		do{
			ret = write(fd,buf,strlen(buf));
		}while(ret < 0 && EINTR == errno);
		if(ret < 0)
		{
			printf("write failed\n");
			continue;
		}
		fsync(fd);
	}
	close(fd);
	return 0;
}

