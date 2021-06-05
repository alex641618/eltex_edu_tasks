#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>

#include <sys/types.h>

#include "my_kalc.h"

int main()
{
	DIR *dir;
	dir = opendir("plugin");
	int pexit = 0, operand1, operand2;
	struct dirent *plugin;
	int counter=0;
	char **lib = (char**) malloc(sizeof(char)*sizeof(char*));
	
	for (int i=0; i<=counter; i++)
		lib[i] = (char*) malloc(sizeof(char));

	do {
		plugin = readdir(dir);
		if (plugin!=NULL && plugin->d_name[0]!='.') {
			lib = (char**) realloc(lib, strlen(plugin->d_name)*(counter+1)*sizeof(char*));
//			for (int i=0; i<=counter; i++)
//				lib[i] = (char*) realloc(lib[i], strlen(plugin->d_name)*sizeof(char));
			lib[counter] = plugin->d_name;
			counter++;
		}
	}
	while (plugin != NULL);

for (int i=0; i<counter; i++)
	printf("%s\n", lib[i]);


	char *libpath = (char*) malloc((strlen(getenv("PWD"))+9)*sizeof(char));
	libpath = getenv("PWD");
	strcat(libpath, "/plugin/");
	//printf("%s\n", libpath);
	char *libname = (char*) malloc(strlen(libpath)*sizeof(char)); 
	char c[2];

	//while (pexit==0) {
		//printf ("Выберите операцию:\n0 - закрытие калькулятора\n1 - сложение\n2 - вычитание\n3 - умножение\n4 - деление\n");
		//scanf("%s", c);
		for(int i = 0; i<counter; i++) {
			libname = realloc(libname, (strlen(libpath) + strlen(lib[i])) *sizeof(char));
			for (int k=0; k<strlen(libpath); k++) {
				libname[k] = libpath[k];
				if (k==strlen(libpath)-1)
					libname[k+1] = '\0';
			}
			strcat(libname, lib[i]);
			printf("%s\n", libname);
			void *dl;
			dl = dlopen(libname, RTLD_NOW);
			if (dl==NULL)
				printf("\n\n\nerror\n\n\n");
			char *string = (char*)malloc(30*sizeof(char));
			string = (char*) dlsym(dl, "op_name");
			printf ("%s\n", string);
		}
	//}
/*
		switch(c[0]) {
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
	*/
	return 0;
}






