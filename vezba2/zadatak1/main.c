#include <stdio.h>

int main()
{
	int a = 8;
	int b = 2;
	int c;
	
	if(a > b)
		c = a - b;
	else
		c = b - a;
	
	printf("Rezultat : %d\n", c);
	
	return 0;
}