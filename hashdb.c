#include "hashdb.h"

hashRecord *record = NULL;

hashRecord *createRecord(uint32_t hash, char* key, uint32_t value)
{
    hashRecord *node = malloc(sizeof(hashRecord));

    if (node == NULL)
        return NULL;

    node->hash = hash;
    strcpy(node->name, key);
    //node->name = key;
    node->salary = value;
    node->next = NULL;

    return node;
}

// Jenkins's one_at_a_time hash function
uint32_t one_at_a_time_hash(const char* key)
{
    // calculate the length in here instead
    size_t length = strlen(key);
    size_t i = 0;
    uint32_t hash = 0;
    while (i != length) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

void *insert_(insert_struct* s)
{
    printf("Hey");
    fflush(stdout);
    // Calculate the hash for this key
    uint32_t hash = one_at_a_time_hash(s->name);

    // write lock
    printf("here");
    fflush(stdout);

    if (record == NULL)
    {
        record = createRecord(hash, s->name, s->salary);
        return;
    }
    printf("here2");
    fflush(stdout);
    if (record->hash == hash)
    {
        record->salary = s->salary;
        return;
    }

    hashRecord *original = record;
    int flag = 0; // if 1 we found it and updated it
    
    while(record->next != NULL)
    {
        record = record->next;

        if (record->hash == hash)
        {
            record->salary = s->salary;
            flag = 1;
            break;
        }
    }

    if (flag != 1)
    {
        record->next = createRecord(hash, s->name, s->salary);
    }

    record = original; // return the original head

    // write lock

    return NULL;
}

void *delete_(char key[])
{
    // Calculate the hash for this key
    uint32_t hash = one_at_a_time_hash(key);

    // write lock

    hashRecord *original = record;
    hashRecord *prev = NULL;
    int flag = 0; // 0 not found, 1 found

    while (record != NULL)
    {
        if (record->hash == hash)
        {
            // Head of list is the desired delete
            if (prev == NULL)
            {
                record = record->next;
                free(original);
            }
            else
            {
                prev->next = record->next;
                hashRecord *temp = record;
                record = original;
                free(temp);
            }
            flag = 1;
            break;
        }
        prev = record;
        record = record->next;
    }

    if (!flag)
    {
        record = original;
    }

    // write lock

    return NULL;
}

void* search_(char key[])
{
    uint32_t hash = one_at_a_time_hash(key);

    // Check if lock is available
    
    hashRecord *original = record;
    hashRecord *prev = NULL;

    while (record != NULL)
    {
        if (record->hash == hash)
        {
            printf("%d, %s, %d\n", record->hash, record->name, record->salary);
            record = original;
            return;
        }
        prev = record;
        record = record->next;
    }
    return NULL;
}
