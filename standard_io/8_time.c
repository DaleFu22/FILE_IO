/*
	funcation:实现每一秒打印一次时间，并同时将打印结果存放在一个文件里
	Usage:
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define SIZE	50

void usage(char *s)
{
	printf("Usage:\n");
	printf("\t%s <dst_file>",s);
	printf("\t<dst_file>:the file need to record the time.\n");
}

int main(int argc, char *argv[])
{
	int i = 0;
	FILE *fp;
	time_t cur_tm;
	struct tm *tm;
	char buf[SIZE];
	if(argc != 2)
	{
		usage(argv[0]);
		exit(1);
	}
	fp = fopen(argv[1],"a+");
	if(fp == NULL)
	{
		fprintf(stderr,"open the file error...\n");
		exit(1);
	}
	while(fgets(buf,BUFSIZ-1,fp) != NULL)
	{
		i++;
	}
	while(1)
	{
		cur_tm = time(NULL);
		tm = localtime(&cur_tm);
		bzero(buf,SIZE);
		snprintf(buf,SIZE-1,"%d.%d--%02d--%02d %02d:%02d:%02d\n",++i,tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
		fputs(buf,fp);
		fflush(fp);
		sleep(1);
		printf("%s",buf);
	}

	fclose(fp);
	return 0;
}

