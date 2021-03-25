#include <stdio.h>

int main() {
	struct test {
		char a;
		int b;
	};

	struct test *my_test;
	char str[10]={'A', 1, 0, 0, 0, 'B', 0, 0, 75, 0};
	my_test=(struct test*) str;
	printf ("my_test.a=%c\nmy_test.b=%d\n", my_test->a, my_test->b);
	printf ("(my_test+1).a=%c\n(my_test+1).b=%d\n", (my_test+1)->a, (my_test+1)->b );
	return 0;
}


