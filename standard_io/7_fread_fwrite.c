/*
	funcation:以块的方式实现拷贝操作
	Usage:./argv[0] src_file dst_file
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>

#undef BUFSIZ
#define BUFSIZ	20

void usage(char *s)
{
	printf("Usage:\n");
	printf("\t%s <src_file> <dst_file>",s);
	printf("\t<src_file>:the source file is to be used in copy\n");
	printf("\t<dst_file>:the destination file is to be used in copy\n");
}

int main(int argc,char **argv)
{
	FILE *fr,*fw;
	long file_size;
	char buf[BUFSIZ];
	if(argc != 3)
	{
		usage(argv[0]);
		exit(1);
	}
	fr = fopen(argv[1],"r");
	fw = fopen(argv[2],"w+");
	if(!fr || !fw)
	{
		fprintf(stderr,"open source file or destination file error...\n");
		exit(1);
	}
	fseek(fr,0,SEEK_END);
	file_size = ftell(fr);
	fseek(fr,0,SEEK_SET);
	//rewind(fr);
	while(file_size >= BUFSIZ-1)
	{
		bzero(buf,BUFSIZ);
		if(fread(buf,BUFSIZ-1,1,fr) < 0)
		{
			perror("fread the source file error...\n");
			continue;
		}
		fwrite(buf,strlen(buf),1,fw);
		file_size -= (BUFSIZ-1);
	}
	if(file_size > 0)
	{
		bzero(buf,BUFSIZ);
		fread(buf,BUFSIZ-1,1,fr);
		fwrite(buf,strlen(buf),1,fw);
	}
	fclose(fr);
	fclose(fw);
}

