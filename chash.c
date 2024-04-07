#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashdb.h"
#include "hashdb.c"


void print_all()
{
    hashRecord *original = record;

    while(record != NULL)
    {
        printf("%d, %s, %d\n", record->hash, record->name, record->salary);
        record = record->next;
    }
    record = original;

    return;
}

void print_search()
{
    
}

int main(void)
{
    // PARSE INPUT

    // if record changes, make sure it gets updated here
    
    char c1[] = "A";
    char c2[] = "B";
    char c3[] = "C";
    char c4[] = "D";
    insert_(c1, 10000);
    insert_(c2, 100);
    insert_(c3, 15000);
    insert_(c4, 5000);
    print_all();
    insert_(c3, 1000);
    search_(c1);
    search_(c3);
    print_all();
    delete_(c3);
    print_all();
    delete_(c4);
    print_all();
    delete_(c1);
    print_all();

    // OUTPUT

    // THESE WILL BE UPDATED WITH VALUES OBTAINED
    int num_lock_aq = 0;
    int num_lock_rel = 0;

    printf("Number of lock aquisitions: %d\n", num_lock_aq);
    printf("Number of lock releases: %d\n", num_lock_rel);
    printf("Final Table:\n");
    print_all(record);
}