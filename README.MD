PA2

Some AI was used for debugging and implementing threading. No large chunks of purely AI generated code were used. The largest chunk of AI generated code was a fixed version of our insert function to sort entries by hash value. Otherwise prompts largely related to asking about error messages and asking why certain segments of our human written code were causing a segfault. The full conversation can be found here: https://chat.openai.com/share/6c11a438-2629-4bd4-93ec-ab3b1adf674d

Some prompts included:

If I need to access a FILE pointer in different files from a main function how do I do so in C?

Why? error: conflicting types for ‘outputFile’; have ‘FILE *’
   27 | extern FILE* outputFile;

   I have the following implementation of insert for a singly linked list. I want it to insert new nodes from least to greatest hash value but it isn't currently working. How do i fix it?
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

    if (record->hash > hash)
    {
        hashRecord *temp = createRecord(hash, s->name, s->salary);
        temp->next = record;
        record = temp;
        rwlock_release_writelock(&mutex);
        return NULL;
    }

    hashRecord *original = record;
    hashRecord *prev = NULL;
    int flag = 0; // if 1 we found it and updated it
    
    while(record->next != NULL)
    {
        if (record->next == hash)
        {
            record->salary = s->salary;
            flag = 1;
            break;
        }
        else if (record->next > hash)
        {
            hashRecord *temp = record->next;
            record->next = createRecord(hash, s->name, s->salary);
            record->next->next = temp;
            flag = 1;
            break;
        }
        else
        {
            record = record->next;
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

   This is the code that is calling createRecord why is there a segfault?
printf("Create head\n");
        fflush(stdout);
        hashRecord *temp = createRecord(hash, s->name, s->salary);
            if (temp == NULL)
            {
                printf("ERRROR");
                fflush(stdout);
            }
            //record = createRecord(hash, s->name, s->salary);
        printf("%u,%s,%s\n",hash,s->name,s->salary);

(I would like to point out that the last prompt yielded no useful results sadly(It was the print statement))