#include <stdio.h>
#include <linux/unistd.h>
int main() {
	char c[1], poisk[11];
	int kol=0, exit=0, met=0, chel=0;
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
		printf("3 - удалить фбонента из списка\n");
		printf("4 - выйти из программы\n");
		fgets(c, 2, stdin);
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
		switch (c[0]) {
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
			fgets((char*) my_sprav[chel].first_name, 21, stdin);
			printf("введите фамилию\n");
                        fgets((char*) my_sprav[chel].second_name, 21, stdin);
			printf("введите номер телефона\n");
                        fgets((char*) my_sprav[chel].num, 12, stdin);
			chel++;
			break;
			case '2':
			printf("введите номер телефона\n");
			fgets((char*) poisk, 12, stdin);
			for (int i=0; i<10; i++) {
                                for(int j=0; j<11; j++) {
                                        if (poisk[j]==my_sprav[i].num[j])
						kol++;
					else
						break;
                                }
				if (kol==11){
					printf("%s        ", my_sprav[i].first_name);
					printf("%s        ", my_sprav[i].second_name);
					printf("%s", my_sprav[i].num);
					kol=0;
				}
            }
			break;
			case '3':
			exit=1;
			break;
		}
	}
	return(0);
}

