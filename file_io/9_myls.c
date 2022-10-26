/*
 *Usage:实现简易版ls功能
 *	./a.out <src_file>
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

void Usage(char *s)
{
	printf("Usage:\n");
	printf("\t%s <src_file>\n",s);
	printf("\t<src_file>:the file need to show information.\n");
}

int main(int argc,char *argv[])
{
	int ret = -1,i;
	char *p = "xwr";
	struct stat st;
	char time_buf[BUFSIZ];
	char buf[BUFSIZ];
	char mode_buf[10];
	bzero(mode_buf,10);
	bzero(time_buf,BUFSIZ);
	bzero(buf,BUFSIZ);
	if(argc != 2)
	{
		Usage(argv[0]);
		exit(1);
	}

	//-rw-rw-r-- 1 hoostar hoostar   558 Oct 25 11:14 1_open.c
#if 0
	ret = stat(argv[1],&st);
	if(ret < 0)
	{
		perror("stat error.\n");
		exit(1);
	}
#else
	if(stat(argv[1],&st) < 0)
	{
		perror("stat error");
		exit(1);
	}
#endif
	//st_mode获取文件类型和权限
	switch(st.st_mode & S_IFMT)
	{
		case S_IFSOCK :mode_buf[0] = 's';break;
		case S_IFLNK  :mode_buf[0] = 'l';break;
		case S_IFREG  :mode_buf[0] = '-';break;
		case S_IFBLK  :mode_buf[0] = 'b';break;
		case S_IFDIR  :mode_buf[0] = 'd';break;
		case S_IFCHR  :mode_buf[0] = 'c';break;
		case S_IFIFO  :mode_buf[0] = 'p';break;
	}
	for(i = 0; i < 9; i++)
	{
		if(st.st_mode & (1<<i))
		{
			mode_buf[9-i] = p[i%3];
		}
		else
		{
			mode_buf[9-i] = '-';
		}
	}

	//获取用户名和组名
	struct passwd *ud = getpwuid(st.st_uid);
	struct group *gd = getgrgid(st.st_gid);

	//获取时间
	//-rw-rw-r-- 1 hoostar hoostar   558 Oct 25 11:14 1_open.c
	time_t cur_sec = st.st_mtime;
	struct tm *tm = localtime(&cur_sec);
	char *month[]={
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec",
	};
	snprintf(time_buf,BUFSIZ-1,"%s %d %d:%d",month[tm->tm_mon],tm->tm_mday,tm->tm_hour,tm->tm_min);
	
	snprintf(buf,BUFSIZ-1,"%s %ld %s %s %ld %s %s\n",mode_buf,st.st_nlink,ud->pw_name,gd->gr_name,st.st_size,time_buf,argv[1]);

	printf("%s",buf);
	
	return 0;
}
