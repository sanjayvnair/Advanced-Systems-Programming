/*
 ============================================================================
 Name        : mapper.c
 Author      : Sanjay Nair
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc , char *argv[]) {
	
	FILE* fh;
	char t[50];
	char output[100][100];
	

	//open and get the file handle
	fh = fopen(argv[1], "r");

	//check if file exists
	if (fh == NULL){
	    perror("file does not exist\n");
	    return 1;
	}

	int i = 0;
	int j = 0;
	//read line by line
	char line[50];
	int count =0;
	
	while (fgets(line, 50, fh) != NULL)  {
		if (line[strlen(line) - 1] == '\n') {
		    line[strlen(line) - 1] = '\0';
		}
				strcpy(output[i],line);					//Store the output into an array
		count++;
		i++;
	}


	//Sort the array of words
	for (i = 1; i < count; i++) {
		      for (j = 1; j < count; j++) {
		         if (strcmp(output[j - 1], output[j]) > 0) {
		            strcpy(t, output[j - 1]);
		            strcpy(output[j - 1], output[j]);
		            strcpy(output[j], t);
		         }
		      }
		   }

	//Print the contents of the output array
	for( i = 0 ; i < count ; i++)
		printf("(%s,1)\n", (output[i]));
	return 0;
}



