#include <stdio.h>
#include <math.h>

#define N 9

int main() {
	int     a = 1,
		counter = 0,
		status = 0,
		m[N][N],
		l = 0,
		co = 0;
	for (int k = 0; k < 2 * N - 1; k++)
	{
		switch (status) {
		case 0:
			l = counter + co;
			if (a == pow(N, 2) && N % 2 > 0)
				m[N / 2][N / 2] = pow(N, 2);
			if (co == 0)
			{
				for (int j = 0 + co; j < N - co; j++) {
					m[l][j] = a;
					a++;
					counter++;
				}
			}
			else
			{
				for (int j = 0 + co; j < N - co - 1; j++) {
					m[l][j] = a;
					a++;
					counter = N;
				}
			}
			counter--;
			status = 1;
			break;
		case 1:
			l = counter - co;
			if (co == 0)
			{
				for (int i = 0 + co + 1; i < N - co - 1; i++) {
					m[i][l] = a;
					a++;
				}
			}
			else
			{
				for (int i = 0 + co; i < N - co; i++) {
					m[i][l] = a;
					a++;
				}
			}
			status = 2;
			if (a == pow(N, 2) && N % 2 == 0)
				m[N / 2][N / 2 - 1] = pow(N, 2);
			break;
		case 2:
			l = counter - co;
			if (co == 0)
			{
				for (int j = N - 1 - co; j >= 0 + 1 + co; j--) {
					m[l][j] = a;
					a++;
				}
			}
			else
			{
				for (int j = N - co - 2; j >= 0 + co + 1; j--) {
					m[l][j] = a;
					a++;
				}
			}
			status = 3;
			counter = 0;
			break;
		case 3:
			l = counter + co;
			for (int i = N - 1 - co; i >= 0 + co + 1; i--) {
				m[i][l] = a;
				a++;
			}
			status = 0;
			counter = 0;
			co++;
			break;
		}
	}
	for (int k = 0; k < N; k++)
	{
		for (int y = 0; y < N; y++)
		{
			printf("%5d ", m[k][y]);
		}
		printf("\n\n\n");
	}
	return(0);
}