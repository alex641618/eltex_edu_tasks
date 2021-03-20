#include <stdio.h>

#define N 10

int main() {
int a=1;
int m[N][N];
for (int i=0; i<N; i++) {
	for (int j=0; j<N; j++) {
		if (i<N-j-1) {
			m[i][j]=0;
			printf("%5d ", m[i][j]);
			}
		else {
			m[i][j]=1;
			printf("%5d ", m[i][j]);
			}
		}
	printf("\n\n\n");
	}
return(0);
}
