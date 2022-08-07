/*Программа выводит числа типа float от 0 и максимум до 16777216 (2^24)
16777216{10} -> 0|10010111|00000000000000000000000{2}*/

#include <stdio.h>
#include <unistd.h>

int main()
{
	float a = 0;
	while (1)
	{
		printf("a = %f\n", a);
		a++;
		sleep(5);
	}
	return 0;
}
