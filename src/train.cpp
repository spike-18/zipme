#include "lib.hpp"


int train(int argc, char* argv[])
{

    char* dir       = NULL;     
    char* dict_path = NULL;

    for (int i = 1; i < argc; i++)                                                              // Find the name of the directory
        if (strcmp(argv[i-1], "-r") == 0)                                                       // with training data files
            dir = argv[i];

    int file_piv = 1;                                                                           // Find the first argument with
    for (; file_piv < argc; file_piv++)                                                         // filename
        if (strcmp(argv[file_piv-1], "-t") == 0)
            break;


    if (dir != NULL)
    {
        printf("Dictionary 'new.dict' will be created in '%s'.\n\n", dir);
        dict_path = concatenate(dir, "new.dict");
    }
    else
    {
        printf("Dictionary 'default.dict' will be created current dir.\n\n");
        dict_path = strdup("default.dict");
    }


    FILE* dictionary = NULL;
    FILE* training_set = NULL;
    
    if ((dictionary = fopen(dict_path, "w+")) == NULL)
        report(FILE_OPEN_ERROR);
    free(dict_path);

    /// HASH
    HashTable ht;
    ht.table = (HashEntry**) calloc(MAX_DICT_LEN, sizeof(HashEntry*));
    initHashTable(&ht);
    ///

    size_t buf_len  = INIT_BUF_LEN;
    char*  buf      = (char*) calloc(buf_len, sizeof(char));
    char*  buf_new  = buf;
    int    dict_len = 0;

    for (int i = file_piv; i < argc; i++)                                                 // Train the dictionary on each file from the argument list
    {        

        char* file_path = (dir == NULL) ? strdup(argv[i]) : concatenate(dir, argv[i]);
        
        printf("FILE %d: %20s\t", i-file_piv, argv[i]);

        if ((training_set = fopen(file_path, "r")) != NULL)
        {    

            char*  index        = NULL;                                                             // index == -1 is reserved for missing prefix in dictionary
            char*  parent_index = NULL;
            
            char c = (char) fgetc(training_set);     

            while (c != EOF)
            {

                if (dict_len >= MAX_DICT_LEN)
                    break;

                index = find_phrase(&ht, parent_index, c);                               // Recursive search for the longest overlap between

                if (index != NULL)
                    parent_index = index;
                else
                {
                    dict_len++;
                    add_phrase(&buf_new, &ht, parent_index, c);
                    parent_index = NULL;
                }
                c = (char) fgetc(training_set);
            }

            fclose(training_set);

            printf(GRN);
            printf("complete\n");
            printf(STD);
        
        }
        else
        {
            printf(RED);
            printf("error\n");
            printf(STD);
        }

        free(file_path);

    }

    save_dict(dictionary, &ht);
    print_dict(&ht);

    fclose(dictionary);
    free(buf);
    free_hash(&ht);

    return 0;
}



void add_phrase(char** buf_new, HashTable* ht, char* parent_index, char c)
{

    char* entry = *buf_new;
                                                                // Find prefix from the start of
    if (parent_index != NULL)                                     // dictionary, NULL if not found
        *buf_new = print_to_buf(*buf_new, parent_index);

    (*buf_new)[0] = c;
    (*buf_new)[1] = '\0';
    insert(ht, entry);

    *buf_new = *buf_new+2;
}



char* concatenate(const char* dir, const char* name)
{
    if(dir)
    {
        size_t dir_len = strlen(dir);
        size_t nam_len = strlen(name);
        
        char* _dir = (char*) calloc(dir_len+nam_len+2, 1);
        _dir[dir_len+nam_len+1] = '\0';

        strcpy(_dir, dir);
        if (_dir[dir_len-1] != '/')
        {
            _dir[dir_len] = '/';
            strcpy(_dir+dir_len+1, name);
        }
        else
        {
            _dir[dir_len] = '/';
            strcpy(_dir+dir_len, name);
        }

        return _dir;   
    
    }
    else
        return strdup("default.dict");
}

char* print_to_buf(char* buf_new, char* str)
{
    size_t i = 0;
    for (; str[i] != '\0'; i++)
        buf_new[i] = str[i];

    return buf_new+i;
}


void save_dict(FILE* dictionary, HashTable* ht)
{
    char sep = -1;
    for (int i = 0; i < MAX_DICT_LEN; i++)
        if (ht->table[i] != NULL)
        {
            fwrite(&i, sizeof(int), 1, dictionary);
            HashEntry* entry = ht->table[i];
            while (entry != NULL) {
                fwrite(&(entry->uindex), sizeof(int), 1, dictionary);
                fputs(entry->value, dictionary);
                fputc('\0', dictionary);
                
                if (entry->next == NULL)
                    fwrite(&sep, 1, 1, dictionary);
                else
                    fputc('\0', dictionary);   
                
                entry = entry->next;
            }
        }
}