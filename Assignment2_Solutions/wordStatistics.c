#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



pthread_mutex_t rp_lock , sp_lock, file_lock;
pthread_cond_t not_empty, not_full;
pthread_cond_t rp_not_empty, rp_not_full , sp_not_empty, sp_not_full;
int buffer_size = 0;
int globalbuffer_size = 0;
int mapper_count;
int reducer_count;
int summarizer_entry_count;
int summarizer_exit_count;
char ** temp_pool;
FILE *file,*letter_file;

//Creating the data structure for Mapper, Reducer and Summarizer pool
struct mapper_pool{
	int key;
	char value[100000];

};

struct letter_count{
	char key;
	int value;

};

struct reducer_pool{
	int key;
	char value[100000];

};

struct summarizer_pool{
	int key;
	char value[100000];

};

//Creating buffers for each pools that points to their respective data structures

struct letter_count letter[100];
struct mapper_pool *mapper1[100];
struct reducer_pool *reducer1[100];
struct summarizer_pool *summarizer1[100];


int TokenizeString(char *s_String, char s_Token[50][50], char c_Delimiter)
	{
		int j = 0;
		unsigned int i_Offset = 0;
		char b_Flag = 0;
		int count = 0;
		for (i_Offset = 0;i_Offset <= strlen(s_String);i_Offset++)
			{
				if (s_String[i_Offset] != c_Delimiter && s_String[i_Offset] != '\t' && s_String[i_Offset] != '\n' && s_String[i_Offset] != '\0')
				{
					s_Token[count][j] = s_String[i_Offset];
					j++;
					b_Flag = 1;
					continue;
				}
			if (b_Flag)
				{
					s_Token[count][j] = '\0';
					count++;
					j = 0;
					b_Flag = 0;
				}
			}
		return (count - 1);
	}

//Mapper implementation

void *mapper(void *threadid){
	int i;
	int index;
	mapper_count = 0;
	index = (int)threadid;
	char *token;
	char string[100];
	for (i = 0 ; i < 100 ; i++)
		reducer1[i] = (struct reducer_pool *) malloc(sizeof(struct reducer_pool));
   	
   	//Locking the mapper pool
	pthread_mutex_lock(&rp_lock); 
   	while(mapper_count > globalbuffer_size)
   		pthread_cond_wait(&rp_not_full, &rp_lock);
   	
   /* get the first token */
   	token = strtok(mapper1[index]->value, ",");
   	
   /* Iterate through other tokens */
   while( token != NULL ) 
   {  
   	  char prefix[100] = "(";
   	  strcpy(string,token);
   	  strcat(prefix,string);
   	  strcat(prefix,",1");
   	  strcat(prefix,")");
   	  strcat(reducer1[index]->value,prefix);
   	  strcat(reducer1[index]->value,"-");
      token = strtok(NULL, ",");
   }
   	
    //printf("REDUCER  INDEX = %d ....value = %s\n",index, reducer1[index]->value); 
   	mapper_count++;
   	//Unlocking the reducer pool
   	pthread_cond_signal(&rp_not_empty);
   	pthread_mutex_unlock(&rp_lock);	
    pthread_exit(NULL);
}


//Reducer Implementation

void *reducer(void *threadid){
	int i;
	int index;
	reducer_count = 0;
	summarizer_entry_count = 0;
	int word_count = 0;
	index = (int)threadid;
	char *token;
	char string[1000];
	char checkstring[10000];
	int token_count = 0;
	
	for (i = 0 ; i < 100 ; i++)
		summarizer1[i] = (struct summarizer_pool *) malloc(sizeof(struct summarizer_pool));
		
	//Locking the reducer pool
	pthread_mutex_lock(&rp_lock); 
  lp:	if (strlen(reducer1[index]->value) == 0 ){
		pthread_mutex_unlock(&rp_lock);	
		goto lp;
	}else{
		// Locking the summarizer pool

		pthread_mutex_lock(&sp_lock); 
   		while(summarizer_entry_count > globalbuffer_size)
   			pthread_cond_wait(&sp_not_full, &sp_lock);
		
		strcpy(string,reducer1[index]->value);
		char s_Token[50][50];
		memset(s_Token,0,200);
		token_count=TokenizeString(string,s_Token,'-');
		int k;
		letter[index].value = (token_count + 1);
		letter[index].key = string[1];
		strcpy(checkstring,s_Token[0]);
		char sub_string[100];
   		char word[100];
   		char w_count[100];
   		int semi_count;
   		char prefix[100] = "(";
		for(k=0; k <= token_count;k++)
			{
				// Calculating the number of repeated words

				if(strcmp(checkstring,s_Token[k]) == 0){
   			 		word_count++;
   			 		if (k == token_count){
   			 			semi_count=TokenizeString(checkstring,s_Token,',');
   			 			strcpy(sub_string,s_Token[0]);
   			 			semi_count=TokenizeString(sub_string,s_Token,'(');
   			 			strcpy(word,s_Token[0]);
   			 			strcat(prefix,word);
   			 			sprintf(w_count,",%d",word_count);
   			 			strcat(prefix,w_count);
   			 			strcat(prefix,")");
   	  					strcat(summarizer1[index]->value,prefix);
   	  					strcat(summarizer1[index]->value,"|");	
   			 			
   			 		}
   			 			

   			 	}else{
   			 		strcpy(checkstring,s_Token[k]);
   			 		semi_count=TokenizeString(s_Token[k - 1],s_Token,',');
   			 		strcpy(sub_string,s_Token[0]);
   			 		semi_count=TokenizeString(sub_string,s_Token,'(');
   			 		strcpy(word,s_Token[0]);
   			 		strcat(prefix,word);
   			 		sprintf(w_count,",%d",word_count);
   			 		strcat(prefix,w_count);
   			 		strcat(prefix,")");
   	  				strcat(summarizer1[index]->value,prefix);
   	  				strcat(summarizer1[index]->value,"|");
   			 		k--;
   			 		word_count = 0;
   			 		strcpy(prefix,"(");
   			 	}
   			 	
			}
		}
	summarizer_entry_count++;
	//UNlocking the summarizer pool
    pthread_cond_signal(&sp_not_empty);
   	pthread_mutex_unlock(&sp_lock);
    reducer_count++;
    //Unlicking the reducer pool
   	pthread_cond_signal(&rp_not_full);
   	pthread_mutex_unlock(&rp_lock);	
    pthread_exit(NULL);
}


//Summarizer Implementation

void summarizer(void *threadid){

	int i;
	int index;
	summarizer_exit_count = 0;
	int word_count = 0;
	index = (int)threadid;
	char *token;
	int token_count = 0;
	char string[1000];
	pthread_mutex_lock(&sp_lock); 

lp:	if (strlen(summarizer1[index]->value) == 0 ){
		pthread_mutex_unlock(&sp_lock);
		usleep( 10000 );	
		goto lp;
	}

	//printf("SUMMARIZER  INDEX = %d ....value = %s\n",index, summarizer1[index]->value); 
	summarizer_exit_count++;
   	pthread_cond_signal(&sp_not_full);
   	pthread_mutex_unlock(&sp_lock);	
    pthread_exit(NULL);
}

//Mapper pool updater implementation
void mapper_pool_updater(char filename[100]){
	
	printf("file name is %s\n", filename );
	
	//open and get the file handle
	FILE* fh;
	fh = fopen(filename, "r");

	//check if file exists
	if (fh == NULL){
	    perror("file does not exist\n");
	    }

	//read line by line
	int i = 0 ;
	int j;
	char t[50];
	char line[100];
	int count =0;

	while (fgets(line, 50, fh) != NULL)  {
		if (line[strlen(line) - 1] == '\n') {
		    line[strlen(line) - 1] = '\0';
		}
		strcpy(temp_pool[i],line);					//Store the output into an array
		count++;
		i++;
	}

	//Sort the array of words
	for (i = 1; i < count; i++) {
		      for (j = 1; j < count; j++) {
		         if (strcmp(temp_pool[j - 1], temp_pool[j]) > 0) {
		             strcpy(t, temp_pool[j - 1]);
		             strcpy(temp_pool[j - 1], temp_pool[j]);
		             strcpy(temp_pool[j], t);
		         }
		      }
		   }

	char substr[2];
	char temp[2];
	int k = 0;
	int flag = 0;
	for (i = 0 ; i < count ; i++){
		mapper1[i] = (struct mapper_pool *) malloc(sizeof(struct mapper_pool));
		strncpy(substr, temp_pool[i], 1);
    	substr[1] = '\0';
    	if (flag == 0){
    		strcpy(temp,substr);
    		flag = 1;
    		i--;

    		continue;
    	}else{

    		if(strcmp(temp,substr) == 0){
    			strcat(temp_pool[i], ",");
    			mapper1[k]->key = k;
				strcat(mapper1[k]->value , temp_pool[i]);

			}else{
				k++;
				buffer_size++;
				i--;
				flag = 0;
				continue;
			}	
    	}
    	
    }

    
    globalbuffer_size = buffer_size + 1;
    
    /*printf("BUFFER SIZE = %d \n", globalbuffer_size );*/
    
    //mapper pool contents
	/*struct mapper_pool *ptr[k];
        for ( i = 0; i <= k ; i++)
    	printf("key = %d ; value = %s\n", mapper1[i]->key , mapper1[i]->value); */

}


int main(int argc , char *argv[]){
	int i;
	temp_pool = malloc(100 * sizeof(char*));
	
	for (i = 0; i < 100; i++){
		temp_pool[i] = malloc((100) * sizeof(char)); 
    	
	}

	pthread_t mapper_threads[atoi(argv[2])], reducer_threads[atoi(argv[3])], summarizer_threads[atoi(argv[4])] ;
   	int rc;
   	long t;
	mapper_pool_updater(argv[1]);
	
	int map_buffer_size = globalbuffer_size - 1;
	int red_buffer_size = globalbuffer_size - 1;
	int sum_buffer_size = globalbuffer_size - 1;
	pthread_mutex_init( &rp_lock, NULL );
	pthread_cond_init( &rp_not_empty, NULL );
	pthread_cond_init( &rp_not_full, NULL );
	pthread_mutex_init( &sp_lock, NULL );
	pthread_cond_init( &sp_not_empty, NULL );
	pthread_cond_init( &sp_not_full, NULL );
	pthread_mutex_init( &file_lock, NULL );
	
	

	file = fopen("wordCount.txt", "w");
	letter_file = fopen("letterCount.txt", "w");
	if (file == NULL | letter_file == NULL)
	{
    	printf("Error opening file!\n");
    	exit(1);
	}

	//Creating the mapper, reducer, summarizer threads

	while (map_buffer_size >= 0){
         for(t=0; t< atoi(argv[2]); t++){
            rc = pthread_create(&mapper_threads[t], NULL, mapper, (void *)map_buffer_size--);
                       	
            if (map_buffer_size < 0)
                break;
            if (rc){
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
         }
    }
         //printf("GLOBAL : %d\n", globalbuffer_size );
    
    while(red_buffer_size >= 0){
    	for(t=0; t< atoi(argv[3]); t++){
            rc = pthread_create(&reducer_threads[t], NULL, reducer, (void *)red_buffer_size--);
            if (red_buffer_size < 0)
                break;
            if (rc){
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
         }
    }

    while(sum_buffer_size >= 0){
    	for(t=0; t< atoi(argv[4]); t++){
            rc = pthread_create(&summarizer_threads[t], NULL, summarizer, (void *)sum_buffer_size--);
            pthread_join(summarizer_threads[t],NULL);
            if (sum_buffer_size < 0)
                break;
            if (rc){
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }

         }
    }

    int token_count = 0;
	char string[1000];
	int m ;

	//The main thread executing the writing into wordCount and Lettercount files

    for ( m = 0 ; m < globalbuffer_size ; m++){
    	
    	strcpy(string,summarizer1[m]->value);
		char s_Token[50][50];
		memset(s_Token,0,200);
		token_count=TokenizeString(string,s_Token,'|');
		int k;
		int semi_index = 0;
		for( k = 0; k <= token_count; k++){
				
   				fprintf(file, "%s\n", s_Token[k]);
   				
				}
		fprintf(letter_file, "(%c,%d)\n", letter[m].key, letter[m].value );
    }
    	
    
		fclose(file);  
		fclose(letter_file);
		printf("The files have been written in the current directory\n");  
  
	pthread_exit(NULL);
}
