#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hashdb.h"
#include "rwlocks.h"


void print_all()
{
    hashRecord *original = record;

    while(record != NULL)
    {
        printf("%u, %s, %u\n", record->hash, record->name, record->salary);
        record = record->next;
    }
    record = original;

    return;
}

void *testing(void*)
{
    printf("A");
    return NULL;
}

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

// prints a command line
void print_command_line(char *command, char *para2, char *para3) {
    //uppercase the command name
    char para1[8];
    strcpy(para1, command);
    for(int i=0; para1[i]!='\0'; i++)
        para1[i] = toupper((char)para1[i]);

    printf("%s,%s,%s\n", para1, para2, para3);
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
    
    // parameter variables + others
    char parameter1[8], parameter2[51], parameter3[11];
    
    // read and process first line: threads
    read_line(inputFile, parameter1, parameter2, parameter3);
    if (strcmp(parameter1, "threads") != 0) 
    {
      fprintf(stderr, "Error occurred. First command should be \"threads\".");
      exit(0);
    }
    print_command_line(parameter1, parameter2, parameter3);

    rwlock_init(&mutex); 
    
    // read and process the rest of the commands
    int num_of_commands = atoi(parameter2);
    pthread_t threads[num_of_commands];


    for(int i=0; i<num_of_commands; i++) 
    {
        read_line(inputFile, parameter1, parameter2, parameter3);
        print_command_line(parameter1, parameter2, parameter3);

        
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
            pthread_create(&threads[i], NULL, insert_, (void *)s);
        } 
        else{
            pthread_create(&threads[i], NULL, testing, NULL);
        }
        // else if(strcmp(parameter1, "delete") == 0) 
        // {
        //     Pthread_create(threads[i], NULL, delete_, (void*)strdup(parameter2));
        //     //delete_(parameter2);
        // } 
        // else if(strcmp(parameter1, "search") == 0) 
        // {
        //     Pthread_create(threads[i], NULL, search_, (void*)strdup(parameter2));
        //     //search_(parameter2);
        // } 
        // else if(strcmp(parameter1, "print") == 0) 
        // {
        //     print_all();
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

    // OUTPUT

    // THESE WILL BE UPDATED WITH VALUES OBTAINED
    int num_lock_aq = 0;
    int num_lock_rel = 0;

    printf("Number of lock aquisitions: %d\n", num_lock_aq);
    printf("Number of lock releases: %d\n", num_lock_rel);
    printf("Final Table:\n");
    print_all(record);
}
