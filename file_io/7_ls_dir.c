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
#include <dirent.h>
#include "color.h"


void show_file_info(struct stat *st_file,char *name);
char *color_file(char *name,struct stat *stb);

void Usage(char *s)
{
	printf("Usage:\n");
	printf("\t1、\n");
	printf("\t  %s\n",s);
	printf("\t2、\n");
	printf("\t  %s dst_file\n",s);
	printf("\t  dst_file:the file need to ls\n");
}

int main(int argc,char **argv)
{
	int len = -1;
	struct stat st;
	char path[BUFSIZ];
	if(argc != 1 && argc != 2)
	{
		Usage(argv[0]);
	}
	if(argc == 1)/*当不传参数时，路径默认为当前目录下 */
	{
		strcpy(path,".");
	}
	else/*当传参时就要看传的是目录还是文件*/
	{
		len = strlen(argv[1]);
		if(len > BUFSIZ - 1)
		{
			len = BUFSIZ-1;
		}
		strncpy(path,argv[1],len);
	}
	/*获取文件或目录的属性*/
	if(stat(path,&st) < 0)
	{
		perror("stat(path,&st) error...\n");
		exit(1);
	}

	/*判断是文件还是目录*/
	if((st.st_mode & S_IFMT) == S_IFDIR)
	{
		/*判断是目录成功*/
		DIR *dp;
		struct dirent *psd;
		struct stat stb;
		dp = opendir(path);
		chdir(path);
		while((psd = readdir(dp)) != NULL)
		{
			if(stat(psd->d_name,&stb) < 0)
			{
				perror("stat(psd->d_name,&stb)");
				continue;
			}
			
			show_file_info(&stb,psd->d_name);
		}
		closedir(dp);
	}
	else
	{
		/*判断是文件成功*/
		show_file_info(&st,argv[1]);
	}

	return 0;
}

void show_file_info(struct stat *st_file,char *name)
{
	int i = 0;
	char *s = "xwr";
	char buf[BUFSIZ];/*注意这尽量给大一点空间*/
	char tm_buf[50];
	struct tm *tm;
	struct passwd *uid;
	struct group *gid;
	/*st_mode获取文件类型和权限*/
	char mode_buf[10];

	bzero(mode_buf,10);
	/*获取文件类型*/
	switch(st_file->st_mode & S_IFMT)
	{
		case S_IFSOCK:mode_buf[0] = 's';break;
		case S_IFLNK:mode_buf[0] = 'l';break;
		case S_IFREG:mode_buf[0] = '-';break;
		case S_IFBLK:mode_buf[0] = 'b';break;
		case S_IFDIR:mode_buf[0] = 'd';break;
		case S_IFCHR:mode_buf[0] = 'c';break;
		case S_IFIFO:mode_buf[0] = 'p';break;
	}

	/*获取文件权限*/
	for(i = 0;i < 9;i++)
	{
		if(st_file->st_mode & (1<<i))
		{
			mode_buf[9-i] = s[i%3];
		}
		else
		{
			mode_buf[9-i] = '-';
		}
	}
	/*获取用户名和组名：st_uid,st_gid*/
	uid = getpwuid(st_file->st_uid);
	gid = getgrgid(st_file->st_gid);

	/*获取时间st_mtime*/
	bzero(tm_buf,50);
	time_t cur_sec = st_file->st_mtime;
	tm = localtime(&cur_sec);
	const char *month[] = {	
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
	snprintf(tm_buf,50-1,"%s  %d %d:%d",month[tm->tm_mon],tm->tm_mday,tm->tm_hour,tm->tm_min);
	bzero(buf,BUFSIZ);
	snprintf(buf,BUFSIZ-1,"%s  %ld %s %s %ld %s %s",mode_buf,st_file->st_nlink,uid->pw_name,gid->gr_name,st_file->st_size,tm_buf,color_file(name,st_file));
	printf("%s\n",buf);
}

char *color_file(char *name,struct stat *stb)
{
	static char color_buf[50];
	bzero(color_buf,50);

	if(!name || !stb)
	{
		return NULL;
	}
	strcpy(color_buf,name);
	if(stb->st_mode & S_IFMT == S_IFDIR)
	{
		/*如果是目录则打印蓝色*/
		sprintf(color_buf,LIGHT_BLUE"%s"NONE,name);
		return color_buf;
	}
	if((stb->st_mode & S_IXUSR) || (stb->st_mode & S_IXGRP) || (stb->st_mode & S_IXOTH))
	{
		/*只要文件有可执行权限就打印绿色*/
		sprintf(color_buf,LIGHT_GREEN"%s"NONE,name);
		return color_buf;
	}
	/*普通文件什么颜色都没有*/
	return color_buf;
}

