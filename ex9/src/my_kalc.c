#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dlfcn.h>
#include <math.h>
#include <dirent.h>
#include <string.h>

#include <sys/types.h>

#include "my_kalc.h"

int str_in_int(char *s, int *d)
{
	int id=0, lenth=strlen(s);
	if (lenth>11)
		return -1;
	int *pd = (int*) malloc(lenth*sizeof(int));
	if(s[0]!='-') {
		for (int i=0; i<lenth; i++) {
			if (s[i] >= 0x30 && s[i] <= 0x39)		
				pd[i] = s[i] - 0x30;
			else
				return -1;
		}
		for (int k=0, i=lenth; i>=0; i--, k++)
			id = id+pd[i]*pow(10, k-1);
		*d=id;
	}
	else {
		for (int i=0; i<lenth; i++) {
			if (s[i] >= 0x30 && s[i] <= 0x39 && i != 0)		
				pd[i] = s[i] - 0x30;
			else if (i==0) {
				continue;
			}
			else
				return -1;
		}
		for (int k=0, i=lenth; i>=0; i--, k++)
			id = id+pd[i]*pow(10, k-1);
		id *= -1;
	}
		free(pd);
		pd=NULL;
		*d=id;
	return 0;
}


int main()
{
	DIR *dir;
	dir = opendir("plugin");
	int operand1, operand2;
	struct dirent *plugin;
	int counter=0, result;
	char **lib = (char**) malloc(sizeof(char)*sizeof(char*));

	do {
		plugin = readdir(dir);
		if (plugin!=NULL && plugin->d_name[0]!='.') {
			lib = (char**) realloc(lib, strlen(plugin->d_name)*(counter+1)*sizeof(char*));
			lib[counter] = plugin->d_name;
			counter++;
		}
	}
	while (plugin != NULL);

	char *libpath = (char*) malloc((strlen(getenv("PWD"))+9)*sizeof(char));
	char env[1000];
	for (int i=0; i<100; i++)
		env[i] = getenv("PWD")[i];
	for (int i=0; i<100; i++)
		libpath[i] = env[i];
	strcat(libpath, "/plugin/");
	char *libname = (char*) malloc(strlen(libpath)*sizeof(char));
	int ccount = counter;
	int len = 1;
	char *sc = (char*) malloc(100*sizeof(char));
	char soperand1[14], soperand2[14];
	int c;
	void *dl;

	while (1) {
		//printf ("Выберите операцию:\n0 - закрытие калькулятора\n1 - сложение\n2 - вычитание\n3 - умножение\n4 - деление\n");
		printf ("Выберите операцию:\n0 - Закрытие калькулятора\n");
		for(int i = 0; i<counter; i++) {
			libname = (char*) malloc((strlen(libpath) + strlen(lib[i])) *sizeof(char));
			for (int k=0; k<strlen(libpath); k++) {
				libname[k] = libpath[k];
				if (k==strlen(libpath)-1)
					libname[k+1] = '\0';
			}
			strcat(libname, lib[i]);
			dl = dlopen(libname, RTLD_NOW);
			if (dl==NULL)
				printf("\n\n\nerror\n\n\n");
			char op_name[100];
			char *sym = (char*) dlsym(dl, "op_name");
			for (int i=0; i<strlen(sym); i++)
				op_name[i] = sym[i];
			printf ("%d - %s\n", i+1, op_name);
		}

repeat0:		
		fgets(sc, 12, stdin);
		sc[strlen(sc)-1] = '\0';
		if (str_in_int(sc, &c) != 0 || c < 0 || c > counter) {
			printf("Вы ввели не верное значение, попробуйте снова\n");
			goto repeat0; 
		}
		if (c==0)
			goto exit;

		printf("Введите первый операнд\n");
repeat1:
		fgets(soperand1, 12, stdin);
		soperand1[strlen(soperand1)-1] = '\0';
		if (str_in_int(soperand1, &operand1) != 0) {
			printf("Вы ввели не верное значение, попробуйте снова\n");
			goto repeat1; 
		}

		printf("Введите второй операнд\n");
repeat2:
		fgets(soperand2, 12, stdin);
		soperand2[strlen(soperand2)-1] = '\0';
		if (str_in_int(soperand2, &operand2) != 0) {
			printf("Вы ввели не верное значение, попробуйте снова\n");
			goto repeat2; 
		}


		for (int i=0; i<counter; i++) {
			char func_name[10];
			for (int k=0; k<strlen(libpath); k++) {
				libname[k] = libpath[k];
				if (k==strlen(libpath)-1)
					libname[k+1] = '\0';
			}
			strcat(libname, lib[i]);

			dl = dlopen(libname, RTLD_NOW);
			if (dl == NULL)
				printf("error\n");
			char *symbol = (char*) dlsym(dl, "func_name");
			int (*func_ptr)(int, int);
			func_ptr = dlsym(dl, symbol);

			if (i==c-1) {
			
				result = func_ptr(operand1, operand2);
			}
		}
		printf("result = %d\n", result);
		
	}
exit:
	dlclose(dl);
	dl = NULL;
	free(lib);
	lib = NULL;
	free(libpath);
	libpath = NULL;
	free(libname);
	libname = NULL;
	free(sc);
	sc = NULL;


	return 0;
}






