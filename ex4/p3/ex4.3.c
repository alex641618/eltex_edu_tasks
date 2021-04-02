#include <stdio.h>
#include <linux/unistd.h>

#define N 10

char c[1], poisk[11];
int kol=0, pexit=0, met=0, chel=0, ik=1, h=0, w=0;
struct sprav {
	char first_name[20],
	second_name[20],
	num[12];
};
struct sprav my_sprav[N];

int fpoisk(char *poisk)
{
		for (int i=0; i<N; i++) {
	        for(int j=0; j<11; j++) {
	            if (poisk[j]==my_sprav[i].num[j])
					kol++;
	        }
				if (kol==11){
					printf("%-20s", my_sprav[i].first_name);
					printf("%-20s", my_sprav[i].second_name);
					printf("%-11s\n", my_sprav[i].num);
					kol=0;
					return i;
				}
			kol=0;
	    }
	    printf("абонент не найден!\n");
	    return -1;
}

int main() {
	while(pexit!=1) {
		printf("что вы собираетесь сделать?\nвведите соответствующее число:\n");
		printf("0 - посмотреть список абонентов\n");
		printf("1 - добавить абонента в список\n");
		printf("2 - найти абонента по номеру телефона\n");
		printf("3 - удалить абонента из списка\n");
		printf("4 - выйти из программы\n");
		scanf("%s", (char*) &c);
		if (met==0) {
			for (int i=0; i<N; i++) {
				for(int j=0; j<20; j++) {
					my_sprav[i].first_name[j] = 0;
					my_sprav[i].second_name[j] = 0;
				}
				for(int j=0; j<12; j++) {
					my_sprav[i].num[j] = 0;
				}
			}
		met++;
		}
		switch (c[0]) {
			case '0':
			for (int i=0; i<N; i++) {
					printf("%-20s", my_sprav[i].first_name);
					printf("%-20s", my_sprav[i].second_name);
					printf("%-11s\n", my_sprav[i].num);
			}
			break;
			case'1':
			if (chel<N) {
				printf("введите имя\n");
				scanf("%s", (char*) my_sprav[chel].first_name);
				printf("введите фамилию\n");
            	scanf("%s", (char*) my_sprav[chel].second_name);
				printf("введите номер телефона\n");
            	scanf("%s", (char*) my_sprav[chel].num);
				chel++;
			}
			else
				printf("список переполнен!\n");
			break;
			case '2':
			printf("введите номер телефона\n");
			scanf("%s", (char*) poisk);
			fpoisk(poisk);
			break;
			case '3':
			printf("введите номер телефона абонента, которого хотите удалить\n");
			scanf("%s", (char*) poisk);
			w = fpoisk(poisk);
			
			if (w>=0)
				printf("удаляется абонент...\n");
			if (w>=0) {
				while (w+ik<N) {
					for (int j = 0; j<20; j++) {
						my_sprav[w+ik-1].first_name[j] = my_sprav[w+ik].first_name[j];
						my_sprav[w+ik-1].second_name[j] = my_sprav[w+ik].second_name[j];
					}
					for (int j = 0; j<12; j++) {
						my_sprav[w+ik-1].num[j] = my_sprav[w+ik].num[j];
					}
					ik++;
				}
				chel--;
				ik=1;
				for (int j=0; j<20; j++) {
					my_sprav[N-1].first_name[j] = 0;
					my_sprav[N-1].second_name[j] = 0;
				}
				for (int j=0; j<12; j++) {
					my_sprav[N-1].num[j] = 0;
				}
			}
			break;
			case '4':
			pexit=1;
			break;
		}
	}
	return(0);
}
