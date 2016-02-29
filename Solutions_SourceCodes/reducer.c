/*
 ============================================================================
 Name        : reducer.c
 Author      : Sanjay Nair
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int i;

int main(void) {
		
		char input[1000][1000];
		int count = 0;
		char string[500] = "";
		char *token;
		

		i = 0;

		while(1){
			
			if (scanf("%s", input[i]) != EOF)
			{	
				if (i == 0){
					i++;
					count = 1;
					continue;
				}else{
					
					if(1){
						if(strcmp(input[i],input[i - 1]) == 0){
							count++;
						}else{
							//Tokenizing the string to get the specific word
							token = strtok(input[i - 1], "(");
					   		token = strtok(token, ",");
					   		if(token != NULL)						//To check if its a file not found error from mapper
					   			printf("\n(%s,%d)", token, count);	//  which leads to a null value in the token
					   		else
					   			return 1;
					   		count = 1;
					   		
					}
				}
					
					
				}
			}
			
			else

			{	
				
				token = strtok(input[i - 1], "(");
				token = strtok(token, ",");
				if(token != NULL)
					printf("\n(%s,%d)", token, count);
				else
		   			return 1;
		   		printf("\nExiting the program");
				break;
			}

			i++;	
		}
		   printf("\n");
		   exit(0);
		}
	
	









