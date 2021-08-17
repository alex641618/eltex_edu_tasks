#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

char *get_string(int *len) 
{
	printf("write string\n");
	*len = 0;
	int capacity = 1;
	char *s = (char*) malloc(capacity * sizeof(char)); 
	char c = getchar(); 
	while (c != '\n') 
	{ 
		s[(*len)++] = c; 
		if (*len >= capacity)
		{ 
			capacity *= 2; 
			s = (char*) realloc(s, capacity * sizeof(char)); 
		}
		c = getchar(); 
	} 
	s[*len] = '\0'; 
	return s;
}


int main() {
	int i=0, size=0, len;
	char **matrix = (char**) malloc(100*sizeof(char*));
	char *ret = get_string(&len);
	size = (strlen(ret)+1);
	while (strcmp(ret, "exit") != 0) {
		matrix = (char**) realloc(matrix, size*sizeof(char*));	
		matrix[i]=(char*)realloc(matrix[i], (strlen(ret)+1)*sizeof(char));
		strcpy(matrix[i], ret);
		ret = get_string(&len);
		size = size + (strlen(ret)+1);
		i++;

	}	
	for (int j=0; j<i; j++) {
		printf("string%d = %s\n", j, matrix[j]);
	}

	for (int j=0; j<i; j++) {
		free(matrix[j]);
	}
	free(matrix);
	return 0;
}