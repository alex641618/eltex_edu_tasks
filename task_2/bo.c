#include <stdio.h>

int main() {

int a=0x0000dabc;
printf("a = %x\n", a);
for (int i=0; i<8; i++) {
	int b=0;
	b = (a & (0x0000000f << 4*i)) >> 4*i;
	printf("a[%d] = %x\n", i+1, b);
}
a = (a & 0xfffff0ff) | 0x00000c00;
printf("замена третьего байта на c\na = %x\n", a);
return 0;
}
