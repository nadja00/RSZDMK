#include <stdio.h>

int main()
{
	int a = 5;
	int b = 7;
	int c;
	
	if(a > b)
		c = a - b;
	else
		c = b - a;
	
	printf("Rezultat : %d\n", c);
	
	return 0;
}