#include <stdio.h>

void main()
{
	int c, nb, nl, nt;

	nb = 0;
	nl = 0;
	nt = 0;

	while ((c = getchar()) != EOF) 
	{
		if (c == '\b')
			printf("\\b");
		else if (c == '\t')
			printf("\\t");
		else if (c == '\\')
			printf("\\\\");
		else 
			putchar(c);
	}
}