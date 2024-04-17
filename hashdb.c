#include "hashdb.h"

hashRecord *record = NULL;

hashRecord *createRecord(uint32_t hash, char key[], uint32_t value)
{
    hashRecord *node = malloc(sizeof(hashRecord));

    if (node == NULL)
        return NULL;

    node->hash = hash;
    strcpy(node->name, key);
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

// void *insert_(insert_struct* s)
// {
//     printf("1\n");
//     fflush(stdout);
//     // Calculate the hash for this key
//     uint32_t hash = one_at_a_time_hash(s->name);
//     printf("2\n");
//     fflush(stdout);
//     print_command_line(s->param1, s->param2);
//     printf("3\n");
//     fflush(stdout);

//     // write lock
//     rwlock_acquire_writelock(&mutex);
    
//     printf("4\n");
//     fflush(stdout);
//     if (record != NULL)
//     {  
//         int flag = 0;
//         hashRecord *original = record;
//         printf("5\n");
//         fflush(stdout);
//         while(record != NULL)
//         {
//             printf("Loop\n");
//             fflush(stdout);
//             if (record->hash == hash)
//             {
//                 printf("Found\n");
//                 fflush(stdout);
//                 record->salary = s->salary;
//                 flag = 1;
//                 break;
//             }
//             record = record->next;
//         }
//         if (flag != 1)
//         {
//             printf("Not Found\n");
//             fflush(stdout);
            
//             record = createRecord(hash, s->name, s->salary);
//             printf("After Create\n");
//             fflush(stdout);
//         }

//         // if (record->hash == hash)
//         // {
//         //     record->salary = s->salary;
            
//         // }

//         // hashRecord *original = record;
//         // int flag = 0; // if 1 we found it and updated it
        
//         // while(record->next != NULL)
//         // {
//         //     record = record->next;

//         //     if (record->hash == hash)
//         //     {
//         //         record->salary = s->salary;
//         //         flag = 1;
//         //         break;
//         //     }
//         // }

//         // if (flag != 1)
//         // {
//         //     record->next = createRecord(hash, s->name, s->salary);
//         // }
//         printf("%u,%s,%s\n",hash,s->name,s->salary);
//         record = original; // return the original head
//     }
//     else
//     {
//         printf("Create head\n");
//         fflush(stdout);
//         record = createRecord(hash, s->name, s->salary);
//         printf("%u,%s,%s\n",hash,s->name,s->salary);
//     }
//     printf("Before release\n");
//     fflush(stdout);

//     // write lock
//     rwlock_release_writelock(&mutex);
//     printf("7\n");
//     fflush(stdout);

//     return NULL;
// }

void* insert_(insert_struct* s)
{
    // Calculate the hash for this key
    uint32_t hash = one_at_a_time_hash(s->name);

    // write lock

    if (record == NULL)
    {
        record = createRecord(hash, s->name, s->salary);
        return;
    }

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

    return;
}

void delete_(char key[])
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

    return;
}

void search_(char key[])
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
}
