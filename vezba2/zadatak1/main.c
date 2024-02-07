#include <stdio.h>

int Calculation(int a, int b)
{
	if ( a > b)
		return a - b;
	else
		return b - a;
}
int main()
{
	int a = 8;
	int b = 2;
	int c;
	
	c = Calculation(a,b);
	
	printf("Rezultat : %d\n", c);
	
	return 0;
}