#include <stdio.h>
#include <linux/unistd.h>
int main() {
	char c, poisk[11];
	int exit=0, met=0, chel=0;
	struct sprav {
		char first_name[20],
		second_name[20],
		num[11];
	};
	struct sprav my_sprav[10];
	while(exit!=1) {
		printf("что вы собираетесь сделать?\nвведите соответствующее число:\n");
		printf("0 - посмотреть список абонентов\n");
		printf("1 - добавить абонента в список\n");
		printf("2 - найти абонента по номеру телефона\n");
		printf("3 - выйти из программы\n");
		scanf("%c", &c);
		if (met==0) {
			for (int i=0; i<10; i++) {
				for(int j=0; j<20; j++) {
					my_sprav[i].first_name[j] = 0;
					my_sprav[i].second_name[j] = 0;
				}
				for(int j=0; j<11; j++) {
					my_sprav[i].num[j] = 0;
				}
			}
		met++;
		}
		switch (c) {
			case '0':
			for (int i=0; i<10; i++) {
				for(int j=0; j<20; j++) {
					printf("%c", my_sprav[i].first_name[j]);
				}
				printf("	");
				for(int j=0; j<20; j++) {
					printf("%c", my_sprav[i].second_name[j]);
				}
				printf("        ");
				for(int j=0; j<11; j++) {
					printf("%c", my_sprav[i].num[j]);
				}
				printf("\n");
			}
			break;
			case'1':
			printf("введите имя\n");
			scanf("%s", (char*) my_sprav[chel].first_name);
			printf("введите фамилию\n");
                        scanf("%s", (char*) my_sprav[chel].second_name);
			printf("введите номер телефона\n");
                        scanf("%s", (char*) my_sprav[chel].num);
			chel++;
			break;
			case '2':
			printf("введите номер телефона");
			scanf("%s", (char*) poisk);
				for (int i; i<11; i++) {
					if(poisk == my_sprav[i].num)
						printf("нашел");
					else
						printf("none");
				}
			break;
			case '3':
			exit=1;
			break;
		}
	}
	return(0);
}
