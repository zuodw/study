#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int array[] = {23, 12, 14, 6, 18, 2, 9};

#define ARRAY_OF(array) (sizeof(array) / sizeof(array[0]))


//将有二个有序数列a[first...mid]和a[mid...last]合并。  
void mergearray(int a[], int first, int mid, int last, int temp[])  
{  
	int i = first, j = mid + 1;
	int k = 0;  

	while (i <= mid && j <= last)  
	{  
		if (a[i] <= a[j])  
			temp[k++] = a[i++];  
		else  
			temp[k++] = a[j++];  
	}  

	while (i <= mid)  
		temp[k++] = a[i++];  

	while (j <= last)  
		temp[k++] = a[j++];  

	for (i = 0; i < k; i++)  
		a[first + i] = temp[i];  
}  
void mergesort(int a[], int first, int last, int temp[])  
{  
	if (first < last)  
	{  
		int mid = first + (last - first) / 2;  
		mergesort(a, first, mid, temp);    //左边有序  
		mergesort(a, mid + 1, last, temp); //右边有序  
		mergearray(a, first, mid, last, temp); //再将二个有序数列合并  
	}  
}  

void MergeSort(int a[], int n)  
{  
	int *p = (int*) malloc(n * sizeof(int));
	if (p == NULL)  
		return;  
	mergesort(a, 0, n - 1, p);  
	free(p);  
	return;  
}  

void main()
{
	int i = 0;
	int count = ARRAY_OF(array);

	MergeSort(array, count);

	for (; i < count; i++)
	{
		printf("%d  ", array[i]);
	}
	printf("\n");
}