#include <stdio.h>

char func_name[10] = "rat";
char op_name[100] = "Деление";

int rat(int param1, int param2)
{
	int result;
	if (param2 == 0 && param1 != 0) {
		printf("infinity\n");
		return 0;
	} else if (param2 == 0 && param1 ==0) {
		printf("undefinetion\n");
		return 0;
	}

	result = param1/param2;
	return result;
}