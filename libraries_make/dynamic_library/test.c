#include <stdio.h>

void fun(int n)
{
	int i;
	if((i = n/10) != 0)
	{
		fun(i);
	}
	putchar(n%10+'0');
	putchar(n%10+'0');
}

int main(void)
{
	fun(67);
	putchar(1+'b');
	return 0;
}
