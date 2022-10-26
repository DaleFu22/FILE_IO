#include <stdio.h>
#include "sqlite3.h"
#include <stdlib.h>
#include <string.h>

int show_message(void *arg,int column,char **value,char **name)
{
	int i;
	for(i = 0;i < column;i++)
	{
		printf("%s\t = %s\t",name[i],value[i]);
	}
	printf("\n");

	/*注意：这里必须return 0，不然会报错*/	
	return 0;
}

int main(void)
{
	int ret = -1;
	sqlite3 *ppdb = NULL;
	char sql[128] = {0};
	int id,age;
	char name[35];
	//打开或创建数据库
	ret = sqlite3_open("student.db",&ppdb);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_open:%s\n",sqlite3_errmsg(ppdb));//注意这里不能用perror，因为perror()函数对应的每个错误都是库里面的错误码，而sqlite3是外来的。
		exit(1);
	}

	//创建表
	sprintf(sql,"create table if not exists student (id integer,name text,age interger);");
	ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_exec1:%s\n",sqlite3_errmsg(ppdb));//注意这里不能用perror，因为perror()函数对应的每个错误都是库里面的错误码，而sqlite3是外来的。
		exit(1);
	}
	
	//插入数据
	while(1)
	{
		fprintf(stderr,"请输入学号，姓名，年龄:");
		scanf("%d %s %d",&id,name,&age);
		if(id == -1)
		{
			break;
		}
		memset(sql,0,128);
		sprintf(sql,"insert into student  values(%d,'%s',%d);",id,name,age);
		ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
		if(ret != SQLITE_OK)
		{
			printf("sqlite3_exec2:%s",sqlite3_errmsg(ppdb));
			exit(1);
		}
	}

	//查询
	memset(sql,0,sizeof(sql));
	sprintf(sql,"select * from student;");
#if 0
	ret = sqlite3_exec(ppdb,sql,show_message,NULL,NULL);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_exec3:%s\n",sqlite3_errmsg(ppdb));
		exit(1);
	}
#else
	char **result;/*用来保存结果*/
	int row,column;/*行和列*/
	ret = sqlite3_get_table(ppdb,sql,&result,&row,&column,NULL);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_get_table:%s\n",sqlite3_errmsg(ppdb));
		exit(1);
	}
	int i,j;
	int index = column;
	for(i = 0;i < row;i++)/*row行*/
	{
		for(j = 0;j < column;j++)/*column列*/
		{
			printf("%s\t = %s\t",result[j],result[index]);
			index++;
		}
		printf("\n");
	}
#endif

	return 0;
}
