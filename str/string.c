#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

struct AYR {
 char *str;
};


int main()
{
	struct AYR *fa;
	fa->str = (char *) malloc(1 * sizeof(char));
	*(fa->str) = 'H';
	printf("str = %c\n", *(fa->str));
	free(fa->str);
	return 0;
}
