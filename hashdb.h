#ifndef HASHDB_H
#define HASHDB_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rwlocks.h"
#include "chash.h"

typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

typedef struct insert_struct
{
  char name[50];
  uint32_t salary;
  char param1[8];
  char param2[51];
  char param3[11];
} insert_struct;

extern hashRecord *record;

struct hash_struct *createRecord(uint32_t hash, char key[], uint32_t value);

uint32_t one_at_a_time_hash(const char* key);

void *insert_(insert_struct* s); // the key is the name

void *delete_(insert_struct* s); // delete is a keyword and couldn't be used

void *search_(insert_struct* s);

void *print_all(void*);

void print_command_line(char *command, char *para2);





#endif