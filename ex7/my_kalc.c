#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "my_kalc.h"

int main()
{
	int pexit = 0, operand1, operand2;
	char c[2];

	while (pexit==0) {
		printf ("Выберите операцию:\n0 - закрытие калькулятора\n1 - сложение\n2 - вычитание\n3 - умножение\n4 - деление\n");
		scanf("%s", c);
		switch(c[0]){
			case '0':
				pexit++;
				break;
			case '1':
				printf("введите первое слагаемое\n");
				scanf("%d", &operand1);
				printf("введите второе слагаемое\n");
				scanf("%d", &operand2);
				printf("\nresult=%d\n\n", summ(operand1, operand2));
				break;
			case '2':
				printf("введите уменьшаемое\n");
				scanf("%d", &operand1);
				printf("введите вычетаемое\n");
				scanf("%d", &operand2);
				printf("\nresult=%d\n\n", diff(operand1, operand2));
				break;
			case '3':
				printf("введите первый множитель\n");
				scanf("%d", &operand1);
				printf("введите второй множитель\n");
				scanf("%d", &operand2);
				printf("\nresult=%d\n\n", pro(operand1, operand2));
				break;	
			case '4':
				printf("введите делимое\n");
				scanf("%d", &operand1);
				printf("введите делитель\n");
				scanf("%d", &operand2);
				printf("\nresult=%d\n\n", rat(operand1, operand2));
				break;
		}
	}

	return 0;
}






