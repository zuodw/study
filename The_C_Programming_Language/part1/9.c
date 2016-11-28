#include <stdio.h>

void main()
{
	int c, lastc;

	lastc = 'a';
	while ((c = getchar()) != EOF)
	{
		if (c != ' ' || lastc != ' ')
			putchar(c);
		lastc = c;
	}
}