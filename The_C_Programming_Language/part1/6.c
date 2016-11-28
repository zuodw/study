#include <stdio.h>

void main ()
{
	int c;

	while ((c =getchar()) != EOF)
	{
		putchar(c);
		printf("    getchar() != EOF is %d\n", getchar() != EOF);
	}
}