#include <stdio.h>	//预编译时直接展开

#define X 10

//这些注释在编译时将会被去掉
int main(void)
{
	printf("%d\n",X*5);
	printf("Hello World\n");

	return 0;
}
