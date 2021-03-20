#include <stdio.h>

#define N 10

int main() {
int a=1;
int s=0;
int m[N];
for (int i=0; i<N; i++) {
	m[i]=a;
	a++;
	printf("%5d ", m[i]);
	}
printf("\n\n");
for (int i=N-1; i>=N/2; i--) {
	s=m[i];
	m[i]=m[N-(i+1)];
	m[N-(i+1)]= s;
	}
for (int i=0; i<N; i++) {
	printf("%5d ", m[i]);
        }
printf("\n");
return(0);
}
