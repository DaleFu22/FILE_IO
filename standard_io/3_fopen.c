#include <stdio.h>

#define FILE_NAME	"./test.txt"

int main(void)
{
	FILE *fp;
	fp = fopen(FILE_NAME,"a+");//以读写方式打开文件，不存在就创建，以追加方式进行写操作,默认是全缓冲
	if(fp == NULL)
	{
		printf("open file %s error...\n",FILE_NAME);
		fprintf(stdout,"open file %s error...\n",FILE_NAME);//stdout行缓冲，相当于printf
		fprintf(stderr,"open file %s error...\n",FILE_NAME);//stderr无缓冲
	}

	fflush(fp);//强制刷新缓冲区的内容到硬盘

	fclose(fp);//结束，关闭打开的文件，内部操作包含了强制刷新操作
	return 0;
}



