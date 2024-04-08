#ifndef HASHDB_H
#define HASHDB_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

hashRecord *record;

struct hash_struct *createRecord(uint32_t hash, char key[], uint32_t value);

uint32_t one_at_a_time_hash(const char* key);

void insert_(char key[], uint32_t value); // the key is the name

void delete_(char key[]); // delete is a keyword and couldn't be used

void search_(char key[]); // IF USING READ LOCKS CAN'T SEARCH WITH RECORD ITSELF OR IT'LL MESS UP OTHER SEARCH THREADS





#endif