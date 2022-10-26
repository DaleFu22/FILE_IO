#include <stdio.h>

int main(void)
{
	printf("我是行缓冲，但我不加换行符");
	fflush(stdout);
	while(1);
	return 0;
}


