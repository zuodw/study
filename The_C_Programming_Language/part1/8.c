#include <stdio.h>

void main()
{
	int c, nb, nl, nt;

	nb = nl = nt = 0;

	while ((c = getchar()) != EOF) 
	{
		if (c == ' ')
			++nb;
		if (c == '\t')
			++nt;
		if (c == '\n')
			++nl;
		printf("nb = %d, nl = %d, nt = %d\n", nb, nl, nt);
	}
}