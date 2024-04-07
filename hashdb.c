#include "hashdb.h"

hashRecord *record;

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

// THIS IS NOT THE ACTUAL HASH FUNCTION, FOR TESTING ONLY!
uint32_t hash_(char key[])
{
    int i = 0;
    uint32_t hash = 0;
    while(key[i] != '\0')
    {
        hash += (int)key[i++];
    }

    return hash;
}

void insert_(char key[], uint32_t value)
{
    // Calculate the hash for this key
    uint32_t hash = hash_(key);

    // write lock

    if (record == NULL)
    {
        record = createRecord(hash, key, value);
        return;
    }

    if (record->hash == hash)
    {
        record->salary = value;
        return;
    }

    hashRecord *original = record;
    int flag = 0; // if 1 we found it and updated it
    
    while(record->next != NULL)
    {
        record = record->next;

        if (record->hash == hash)
        {
            record->salary = value;
            flag = 1;
            break;
        }
    }

    if (flag != 1)
    {
        record->next = createRecord(hash, key, value);
    }

    record = original; // return the original head

    // write lock

    return;
}

void delete_(char key[])
{
    // Calculate the hash for this key
    uint32_t hash = hash_(key);

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
    uint32_t hash = hash_(key);

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

void main()
{
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
}