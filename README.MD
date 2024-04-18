PA2

Some AI used for debugging and implementing threading. No large chunks of AI generated code were used. Prompts largely related to asking about error messages and asking why certain segments of our human written code were causing a segfault. The full conversation can be found here: https://chat.openai.com/share/6c11a438-2629-4bd4-93ec-ab3b1adf674d

Some prompts included:

If I need to access a FILE pointer in different files from a main function how do I do so in C?

Why? error: conflicting types for ‘outputFile’; have ‘FILE *’
   27 | extern FILE* outputFile;

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