#include <stdint.h>
#include <stdio.h>

static int array[] = {23, 12, 14, 6, 18, 2, 9};

#define ARRAY_OF(array) (sizeof(array) / sizeof(array[0]))

void insertion_sort(int array[], int count)
{
	int i, j, temp;
	for (i = 1; i < count; i++)
	{
		for (j = i; j > 0; j--)
		{
			if (array[j] < array[j - 1])
			{
				temp = array[j];
				array[j] = array[j - 1];
				array[j - 1] = temp;
			}
		}
	}
}

void main()
{
	int i = 0;
	int count = ARRAY_OF(array);
	insertion_sort(array, count);
	for (; i < count; i++)
	{
		printf("%d  ", array[i]);
	}
	printf("\n");
}