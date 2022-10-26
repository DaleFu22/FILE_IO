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
	bzero(buf,BUFSIZ);
	while(fgets(buf,BUFSIZ-1,fr) != NULL)
	{
		if(fputs(buf,fw) == EOF)
		{
			perror("fputs error");
		}
		bzero(buf,BUFSIZ);
	}
	fflush(fw);
//	while(1);
	fclose(fr);
	fclose(fw);
}

