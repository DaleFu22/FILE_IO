#include <stdio.h>

int main(void)
{
	fopen("./test.txt","a+");//以读写方式打开文件，不存在就创建，以追加方式进行写操作

	return 0;
}



