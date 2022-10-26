/*
funcation:复制ascii文件
   usage:
   ./fgetc_fputc  src_file  dst_file
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void usage(char *s)
{
	printf("usage:\t\n");
	printf("\t%s src_file dst_file\n",s);
	printf("\tsrc_file:the source file is need to copy\n");
	printf("\tdst_file:the destnation file is need to copy\n");
}

int main(int argc,char *argv[])
{
	/*检查传递的参数的正确性*/
	FILE *fr,*fw;
	int ch;
	
	if(argc != 3)
	{
		usage(argv[0]);
		exit(1);
	}

	fr = fopen(argv[1],"r");
	fw = fopen(argv[2],"w");
	if(fr == NULL || fw == NULL)
	{
		fprintf(stderr,"open source file or destination file error...\n");
		exit(1);
	}
#if 0
	do
	{
		ch = fgetc(fr);
		fputc(ch,fw);
	}while(!feof(fr) && !ferror(fr));//读取的文件既没有到达文件末尾也没有出错
#else
	ch = fgetc(fr);
	while(!feof(fr) && !ferror(fr))
	{
		fputc(ch,fw);
		ch = fgetc(fr);
		fflush(fw);//如果不加fflush()的话，如果后面造成一个死循环，则数据写进去，一直在缓冲区里
	}
	printf("%d\n",ch);
//	while(1);
#endif
	fclose(fr);
	fclose(fw);
}

