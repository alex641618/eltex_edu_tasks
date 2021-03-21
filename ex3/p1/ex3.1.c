#include <stdio.h>

int main()
{
int a=270533154;
char* ptr=&a;
printf("a = %d a=0x%x\n", a, a);
printf("first byte of a = %d	first byte of a = 0x%x\n", *ptr, *ptr);
ptr++;
printf("second byte of a = %d	second byte of a = 0x%x\n", *ptr, *ptr);
ptr++;
printf("thirh byte of a = %d	third byte of a = 0x%x\n", *ptr, *ptr);
ptr++;
printf("fourth byte of a = %d	fourth byte of a = 0x%x\n", *ptr, *ptr);
return(0);
}
