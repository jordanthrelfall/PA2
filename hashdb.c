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

void test_print()
{
    hashRecord *original = record;

    while(record != NULL)
    {
        printf("%d %s %d -> ", record->hash, record->name, record->salary);
        record = record->next;
    }
    printf("\n");
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
    test_print();
    insert_(c3, 1000);
    test_print();
    delete_(c3);
    test_print();
    delete_(c4);
    test_print();
    delete_(c1);
    test_print();
}