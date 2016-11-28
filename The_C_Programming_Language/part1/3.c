#include <stdio.h>

void main()
{
	float fahr, celsius;
	int lower, upper, step;

	lower = 0;
	upper = 300;
	step = 20;

	printf("fahr  celsius\n");
	fahr = lower;

	while (fahr <= upper)
	{
		celsius = (5.0 / 9.0) * (fahr - 32.0);
		printf("%3.0f   %6.1f\n", fahr, celsius);
		/* %6.1f:按照浮点数打印，至少6个字符宽，小数点后保留1位 */
		fahr += step;
	}
}