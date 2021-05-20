#include <stdio.h>
#include <linux/unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

char *c, *poisk, *buffer;
int kol=0, pexit=0, met=0, chel=0, ik=1, h=0, w=0, len, *N, first=0, second=0, a=1;
struct sprav {
	char *first_name,
	*second_name,
	*num;
};

struct sprav **my_sprav;


int get_string(int *len) 
{
	N=&a;
	*len = 0;
	buffer = (char*) malloc(*N * sizeof(char));
	char c = getchar(); 

	while (c != '\n') 
	{ 
		buffer[(*len)++] = c;

		if (*len >= *N)
		{ 
			*N *= 2;  
			buffer = (char*) realloc(buffer, *N * sizeof(char));
		}

		c = getchar();
	} 

	*N=*len+1;
	buffer = (char*) realloc(buffer, *N * sizeof(char));
	buffer[*len] = '\0';
	return *len+1;
}


int fpoisk(char *poisk)
{

		for (int i=0; i<chel; i++) {

	        for(int j=0; j<strlen(poisk); j++) {

	            if (poisk[j]==my_sprav[i]->num[j])
					kol++;

	        }

				if (kol==strlen(poisk)){
					printf("%-20s", my_sprav[i] -> first_name);
					printf("%-20s", my_sprav[i] -> second_name);
					printf("%-11s\n", my_sprav[i] -> num);
					kol=0;
					return i;
				}

			kol=0;
	    }

	    printf("абонент не найден!\n");
	    return -1;
}


int main() {
	c = (char*) malloc(sizeof(char));

	while(pexit!=1) {
		printf("что вы собираетесь сделать?\nвведите соответствующее число:\n");
		printf("0 - посмотреть список абонентов\n");
		printf("1 - добавить абонента в список\n");
		printf("2 - найти абонента по номеру телефона\n");
		printf("3 - удалить абонента из списка\n");
		printf("4 - выйти из программы\n");
		get_string(&len);
		c = buffer; 

		switch (*c) {

			case '0':
				for (int i=0; i<chel; i++) {
						printf("%-20s", my_sprav[i]->first_name);
						printf("%-20s", my_sprav[i]->second_name);
						printf("%-11s\n", my_sprav[i]->num);
				}

			break;

			case'1':
				printf("введите имя\n");

				if (chel == 0) {
					my_sprav = (struct sprav**) malloc (get_string(&len) * sizeof(struct sprav*));
					my_sprav[chel] = (struct sprav*) malloc (a * sizeof( char *));
					my_sprav[chel]->first_name = (char *) malloc (a * sizeof( char ));

						for (int i=0; i<a; i++){
							my_sprav[chel]->first_name[i] = buffer[i];
						}

					first = a;
					free(buffer);
					}

				else {
					my_sprav = (struct sprav**) realloc (my_sprav, chel * get_string(&len) * sizeof(struct sprav*));
					my_sprav[chel] = (struct sprav*) malloc (a * sizeof( char *));
					my_sprav[chel]->first_name = (char *) malloc (a * sizeof( char ));

					for (int i=0; i<a; i++){
						my_sprav[chel]->first_name[i] = buffer[i];
					}

					first = a;
					free(buffer);
				}

				printf("введите фамилию\n");
				my_sprav = (struct sprav**) realloc (my_sprav, (first + get_string(&len)) * sizeof(struct sprav*));
				my_sprav[chel]->second_name = (char *) malloc (a * sizeof( char ));
			
				for (int i=0; i<a; i++){
					my_sprav[chel]->second_name[i] = buffer[i];
				}

				second = a;
				free(buffer);
				printf("введите номер телефона\n");
				my_sprav = (struct sprav**) realloc (my_sprav, (first + second + get_string(&len)) * sizeof(struct sprav*));
    	        my_sprav[chel]->num = (char*) malloc (a * sizeof( char ));

    	       	for (int i=0; i<a; i++){
					my_sprav[chel]->num[i] = buffer[i];
				}

				chel++;
				free(buffer);
				break;

			case '2':
				printf("введите номер телефона\n");
				poisk = (char*) malloc(get_string(&len) * sizeof(char));

				for (int i=0; i<a; i++) {
					poisk[i]=buffer[i];
				}
			
				fpoisk(poisk);
				free(poisk);
				break;

			case '3':
				printf("введите номер телефона абонента, которого хотите удалить\n");
			
				poisk = (char*) malloc(get_string(&len) * sizeof(char));

				for (int i=0; i<a; i++) {
					poisk[i]=buffer[i];
				}
				
				w = fpoisk(poisk);
				free(poisk);				

				if (w>=0) {
					printf("удаляется абонент...\n");

					while (w+ik<chel) {
					
						for (int j = 0; j<sizeof(my_sprav[w+ik-1]->first_name); j++) {
							my_sprav[w+ik-1]->first_name[j] = my_sprav[w+ik]->first_name[j];
						}

						for (int j = 0; j<sizeof(my_sprav[w+ik-1]->second_name); j++) {
							my_sprav[w+ik-1]->second_name[j] = my_sprav[w+ik]->second_name[j];
						}

						for (int j = 0; j<sizeof(my_sprav[w+ik-1]->num); j++) {
							my_sprav[w+ik-1]->num[j] = my_sprav[w+ik]->num[j];
						}

						ik++;
					}

					chel--;
					ik=1;
						free(my_sprav[chel]->first_name);
						free(my_sprav[chel]->second_name);
						free(my_sprav[chel]->num);
				}
				break;

			case '4':
				pexit=1;
				break;

		}

	}

	for (int i; i<chel; i++)
	{
		free(my_sprav[i]->first_name);
		free(my_sprav[i]->second_name);
		free(my_sprav[i]->num);
		free(my_sprav[i]);
	}

	free(my_sprav);
	free(c);

	return(0);
}
