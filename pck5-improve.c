#include <stdio.h>

int pwr[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 1024};

int main(void)
{	int n;
	int s = 0;

	scanf("%d", &n);

	for(int i = 0; pwr[i] < n; i++)
		s += pwr[i];

	printf("%d\n", s);

	return 0;
}
