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

void print_command_line(char *command, char *para2) {
    //uppercase the command name
    char para1[8];
    strcpy(para1, command);
    for(int i=0; para1[i]!='\0'; i++)
        para1[i] = toupper((char)para1[i]);

    if (strcmp(para1, "INSERT") == 0)
    {
        fprintf(outputFile,"%s,",para1);
    }
    else if (strcmp(para1, "SEARCH") == 0)
    {
        fprintf(outputFile,"%s,%s\n", para1,para2);
    }
    else if (strcmp(para1, "DELETE") == 0)
    {
        fprintf(outputFile,"%s,%s\n",para1, para2);
    }
}

void* insert_(insert_struct* s)
{
    // Calculate the hash for this key
    uint32_t hash = one_at_a_time_hash(s->name);
    print_command_line(s->param1, s->param2);
    fprintf(outputFile,"%u,%s,%s\n",hash,s->param2,s->param3);

    // write lock
    rwlock_acquire_writelock(&mutex);

    if (record == NULL)
    {
        record = createRecord(hash, s->name, s->salary);
        rwlock_release_writelock(&mutex);
        return NULL;
    }

    if (record->hash == hash)
    {
        record->salary = s->salary;
        rwlock_release_writelock(&mutex);
        return NULL;
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
    rwlock_release_writelock(&mutex);

    return NULL;
}

void *delete_(insert_struct* s)
{
    // Calculate the hash for this key
    uint32_t hash = one_at_a_time_hash(s->name);
    print_command_line(s->param1,s->param2);
    // write lock
    rwlock_acquire_writelock(&mutex);
    if (record != NULL)
    {
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
    }

    
    rwlock_release_writelock(&mutex);
    // write lock

    return NULL;
}

void* search_(insert_struct* s)
{
    uint32_t hash = one_at_a_time_hash(s->name);
    print_command_line(s->param1,s->param2);
    // acquire read lock
    rwlock_acquire_readlock(&mutex);

    if (record != NULL)
    {
        hashRecord *original = record;
        hashRecord *prev = NULL;

        while (original != NULL)
        {
            if (original->hash == hash)
            {
                fprintf(outputFile,"%d,%s,%d\n", original->hash, original->name, original->salary);
                rwlock_release_readlock(&mutex);
                return NULL;
            }
            prev = original;
            original = original->next;
        }
    }
    // release read lock
    rwlock_release_readlock(&mutex);
    
    return NULL;
}

void* print_all(void*)
{
    // acquire read lock
    rwlock_acquire_readlock(&mutex);
    if (record != NULL)
    {
        hashRecord *original = record;

        while(original != NULL)
        {
            fprintf(outputFile,"%u,%s,%u\n", original->hash, original->name, original->salary);
            original = original->next;
        }
    }
    // release read lock
    rwlock_release_readlock(&mutex);
    
    return NULL;
}
