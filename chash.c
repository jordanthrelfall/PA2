#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hashdb.h"
#include "rwlocks.h"

// reads and splits the command ling into 3 parameters
void read_line(FILE *input, char *para1, char *para2, char *para3) {
    char input_line[70], ch; //7+2+50+10+'\0'=70
    fscanf(input, "%[^,\n]%c", input_line, &ch);
    strcpy(para1, input_line);
    fscanf(input, "%[^,\n]%c", input_line, &ch);
    strcpy(para2, input_line);
    fscanf(input, "%[^,\n]%c", input_line, &ch);
    strcpy(para3, input_line);
}

int main(void)
{
    // PARSE INPUT
    //open input file
    FILE *inputFile = fopen("commands.txt", "r");
    if (inputFile == NULL) 
    {
      fprintf(stderr, "Could not open file. Error occurred.");
      exit(0);
    }
    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Cold not open file. Error occured.\n");
        return 1;
    }
    
    // parameter variables + others
    char parameter1[8], parameter2[51], parameter3[11];
    
    // read and process first line: threads
    read_line(inputFile, parameter1, parameter2, parameter3);
    if (strcmp(parameter1, "threads") != 0) 
    {
      fprintf(stderr, "Error occurred. First command should be \"threads\".");
      exit(0);
    }
    //print_command_line(parameter1, parameter2, parameter3);
    fprintf(outputFile, "Running %s threads\n", parameter2);

    rwlock_init(&mutex); 
    
    // read and process the rest of the commands
    int num_of_commands = atoi(parameter2);
    pthread_t threads[num_of_commands];


    for(int i=0; i<num_of_commands; i++) 
    {
        read_line(inputFile, parameter1, parameter2, parameter3);
        //printf("%s,%s,%s\n",parameter1,parameter2,parameter3);
        //print_command_line(parameter1, parameter2);

        
        if(strcmp(parameter1, "insert") == 0) 
        {
            char* endptr;
            insert_struct* s = (insert_struct*)malloc(sizeof(insert_struct));
            if (s == NULL)
            {
                fprintf(stderr, "Memory Allocation Error");
            }
            strcpy(s->name, parameter2);
            s->salary = (uint32_t)strtoul(parameter3, &endptr, 10);
            strcpy(s->param1, parameter1);
            strcpy(s->param2, parameter2);
            strcpy(s->param3, parameter3);
            //pthread_create(&threads[i], NULL, insert_, (void *)s);
            insert_(s);
        } 
        // else if (strcmp(parameter1, "delete") == 0)
        // {
        //     insert_struct* s = (insert_struct*)malloc(sizeof(insert_struct));
        //     if (s == NULL)
        //     {
        //         fprintf(stderr, "Memory Allocation Error");
        //     }
        //     strcpy(s->name, parameter2);
        //     s->salary = (uint32_t)0;
        //     strcpy(s->param1, parameter1);
        //     strcpy(s->param2, parameter2);
        //     strcpy(s->param3, parameter3);
        //     pthread_create(&threads[i], NULL, delete_, (void *)s);
        // }
        // else if (strcmp(parameter1, "search") == 0)
        // {
        //     insert_struct* s = (insert_struct*)malloc(sizeof(insert_struct));
        //     if (s == NULL)
        //     {
        //         fprintf(stderr, "Memory Allocation Error");
        //     }
        //     strcpy(s->name, parameter2);
        //     s->salary = (uint32_t)0;
        //     strcpy(s->param1, parameter1);
        //     strcpy(s->param2, parameter2);
        //     strcpy(s->param3, parameter3);
        //     pthread_create(&threads[i], NULL, search_, (void *)s);
        // }
        // else if(strcmp(parameter1, "print") == 0) 
        // {
            
        //     pthread_create(&threads[i], NULL, print_all, NULL);
            
        // } 
        // else {
        //     fprintf(stderr, "Error occurred. Invalid command: %s", parameter1);
        //     exit(0);
        // }
    }


    for (int i = 0; i < num_of_commands; i++)
    {
        pthread_join(threads[i], NULL);
    }

    print_all(NULL);

    // OUTPUT

    // THESE WILL BE UPDATED WITH VALUES OBTAINED
    int num_lock_aq = 0;
    int num_lock_rel = 0;

    printf("Number of lock aquisitions: %d\n", num_lock_aq);
    printf("Number of lock releases: %d\n", num_lock_rel);
    printf("Final Table:\n");
    print_all(record);
}
