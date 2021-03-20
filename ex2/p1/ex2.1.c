#include <stdio.h>

#define N 10

int main() {
int a=1;
int m[N][N];
for (int i=0; i<N; i++) {
	for (int j=0; j<N; j++) {
		m[i][j]=a;
		a++;
		printf("%5d ", m[i][j]);
		}
	printf("\n\n\n");
	}
return(0);
}
