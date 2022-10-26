#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>

void Usage(char *s)
{
	printf("Usage:\n");
	printf("\t%s dst_file\n",s);
	printf("\tdst_file:the destination file need to show\n");
}

int main(int argc,char **argv)
{
	int i = 0;
	char *s = "xwr";
	char tm_buf[50];
	char buf[BUFSIZ];
	struct stat st;
	char mode_buf[10] = {0};
	if(argc != 2)
	{
		Usage(argv[0]);
		exit(1);
	}
	
	if(stat(argv[1],&st) < 0)
	{
		perror("error getting file properties.\n");
		exit(1);
	}

	/*st_mode:获取文件的类型与权限*/
	switch(st.st_mode & S_IFMT)
	{
		case S_IFSOCK:mode_buf[0] = 's';break;
		case S_IFLNK:mode_buf[0] = 'l';break;
		case S_IFREG:mode_buf[0] = '-';break;
		case S_IFBLK:mode_buf[0] = 'b';break;
		case S_IFDIR:mode_buf[0] = 'd';break;
		case S_IFCHR:mode_buf[0] = 'c';break;
		case S_IFIFO:mode_buf[0] = 'p';break;
	}

	for(i = 0;i < 9;i++)
	{
		if(st.st_mode & (1<<i))
		{
			mode_buf[9-i] = s[i%3];
		}
		else
		{
			mode_buf[9-i] = '-';
		}
	}

	/*st_uid:获取用户名st_gid:获取组名*/
	struct passwd *username = getpwuid(st.st_uid);
	struct group *grpname = getgrgid(st.st_gid);

	/*st_mtime:获取时间*/
	bzero(tm_buf,50);
	time_t m_sec = st.st_mtime;
	struct tm *tm = localtime(&m_sec);
	const char *month[]={
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
	
	sprintf(tm_buf,"%s  %d %d:%d",month[tm->tm_mon],tm->tm_mday,tm->tm_hour,tm->tm_min);

	bzero(buf,BUFSIZ);
	sprintf(buf,"%s %ld %s %s %ld %s %s\n",mode_buf,st.st_nlink,username->pw_name,grpname->gr_name,st.st_size,tm_buf,argv[1]);
	printf("%s",buf);

	return 0;
}
