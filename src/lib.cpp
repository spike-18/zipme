#include "lib.hpp"
#include "compress.cpp"
#include "decompress.cpp"
#include "train.cpp"
#include "hash.cpp"


char* find_phrase(HashTable* ht, char* parent_index, char c)
{

    if (parent_index != NULL)
    {
        char str[MAX_STRING_LEN] = "";
        strcat(str, parent_index);
        char charStr[2] = {c, '\0'};
        strcat(str,charStr);
        
        char* ind = retrieve(ht, str);
        if (ind != NULL) //
            return ind;
    }

    else
    {
        char charStr[2] = {c, '\0'};
        char* ind = retrieve(ht, charStr);
        if (ind != NULL) //
            return ind;
    }
    
    return NULL;
}


void initHashTable(HashTable *ht) {
    for (size_t i = 0; i < MAX_DICT_LEN; ++i) {
        ht->table[i] = NULL;
    }
}


void insert(HashTable *ht, char *key) {
    unsigned int index = fnv1_hash(key);
    HashEntry *newEntry = (HashEntry *)malloc(sizeof(HashEntry));
    if (newEntry == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newEntry->value = key;
    newEntry->next = NULL; // Initialize next pointer to NULL

    if (ht->table[index] == NULL)
    {
        newEntry->uindex = index;
        ht->table[index] = newEntry;
    }
    else 
    {
        newEntry->uindex = replace(ht, key);
        HashEntry *current = ht->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newEntry;
    }

}


char* retrieve(HashTable *ht, const char *key) {
    unsigned int index = fnv1_hash(key);
    HashEntry* entry = ht->table[index];
    while (entry != NULL) {
        if (strcmp(entry->value, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL; // Key not found
}


int replace(HashTable* ht, char* key)
{
    unsigned int index = hash(key);
    unsigned int nextIndex = (index + 1) % MAX_DICT_LEN;
    unsigned int prevIndex = (index - 1) % MAX_DICT_LEN;

    while (nextIndex != prevIndex)
        if (ht->table[nextIndex]==NULL)
        {
            HashEntry *newEntry = (HashEntry *)malloc(sizeof(HashEntry));
            if (newEntry == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            newEntry->value = key;
            newEntry->uindex = nextIndex;
            newEntry->next = NULL;
            ht->table[nextIndex] = newEntry;
            return nextIndex;
        }  
        else if (ht->table[prevIndex]==NULL)
        {
            HashEntry *newEntry = (HashEntry *)malloc(sizeof(HashEntry));
            if (newEntry == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            newEntry->value = key;
            newEntry->uindex = prevIndex;
            newEntry->next = NULL;
            ht->table[prevIndex] = newEntry;
            return prevIndex;
        }
        else
        {
            nextIndex = (nextIndex + 1) % MAX_DICT_LEN;
            prevIndex = (prevIndex - 1) % MAX_DICT_LEN;
        }


    return 0;
}


int get_index (HashTable* ht, const char* key)
{
    if (key == NULL)
        return -1;
    unsigned int index = fnv1_hash(key);
    HashEntry* entry = ht->table[index];
    while (entry != NULL) {
        if (strcmp(entry->value, key) == 0) {
            return entry->uindex;
        }
        entry = entry->next;
    }

    return -1;
}



void read_dict (FILE* dictionary, HashTable* ht, char* buf)
{
    int index = 0;
    int i = 0;
    char c;
    
    while (1)
    {

        if (fread(&index, sizeof(int), 1, dictionary) != 1)
            break;

        do
        {
            HashEntry *newEntry = (HashEntry *)malloc(sizeof(HashEntry));
            fread(&(newEntry->uindex), sizeof(int), 1, dictionary);
            newEntry->value = buf + i;

            c = (char) fgetc(dictionary);
            
            while(c!='\0')
            {
                buf[i] = c;
                c = (char) fgetc(dictionary);
                i++;
            }
            buf[i] = '\0';
            i++;

            insert_entry(ht, index, newEntry);

            fread(&c, 1, 1, dictionary);

        } while (c != -1);
        

    }
                
}

void insert_entry(HashTable* ht, int index, HashEntry* entry)
{
    entry->next = NULL;
    
    if (ht->table[index] == NULL)
    {
        ht->table[index] = entry;
    }
    else 
    {
        HashEntry *current = ht->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = entry;
    }

}


void print_dict(HashTable* ht)
{
    int counter=0;
    for (int i = 0; i < MAX_DICT_LEN; i++)
        if (ht->table[i] != NULL)
        {
            counter++;
            // printf("INDEX %d", i);
            // HashEntry* entry = ht->table[i];
            // while (entry != NULL) {
            //     printf("\n\tentry UID %d : %s", entry->uindex, entry->value);
            //     entry = entry->next;
            // }
            // printf("\n");
        }

    printf("\n~~~TOTAL~~~\n");
    printf("%d / %d\n\n", counter, MAX_DICT_LEN);


    // int counter=0;
    // for (int i = 0; i < MAX_DICT_LEN; i++)
    //     if (ht->table[i] == NULL)
    //     {
    //         counter++;
    //     }
    // printf("Dict NULL: %d / %d\n", counter, MAX_DICT_LEN);
    
}


void free_hash(HashTable *ht) {

    for (int i = 0; i < MAX_DICT_LEN; i++) {
        HashEntry *current = ht->table[i];

        while (current != NULL) {
            HashEntry *temp = current;
            current = current->next;
            free(temp);
        }
        ht->table[i] = NULL; 
    }
}



int setmode(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0)
            return DECOMPRESS;
        else if (strcmp(argv[i], "-t") == 0)
            return TRAIN;
    }

    return COMPRESS;
}


void checkargs(int argc, char* argv[])
{
    // Searches for expression mistakes
    if (argc < 2)
        report(NO_INPUT_FILE);
    
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            print_help();
            exit(0);
        }   
        if (strcmp(argv[i], "-d") == 0)
        {
            if (i+1 < argc && strncmp(argv[i+1], "-", 1) == 0)
                report(NO_INPUT_FILE);
        }
        if (i+1 < argc && strcmp(argv[i], "-D") == 0)
        {
            if (strncmp(argv[i+1], "-", 1) == 0)
                report(NO_DICT);
            else if (i+2 > argc-1)
                report(INV_SYNT);
        }
        if (strcmp(argv[i], "-t") == 0)
        {
            if (i == argc-1)
                report(NO_INPUT_FILE);
        }
        if (strcmp(argv[i], "-r") == 0)
        {
            if (i+1 < argc && strncmp(argv[i+1], "-", 1) == 0)
                report(NO_DIRECTORY);
            else if (i+2 >= argc-1 && strncmp(argv[i+2], "-", 1))
                report(MULT_DIR);
        }
    }
    
}


void report(int error)
{
    printf(RED);
    switch (error)
    {
    case NO_INPUT_FILE:
        printf("No input file(s).\n"); 
        break;
    case NO_DICT:
        printf("No dictionary file proceeded.\n"); 
        break;
    case INV_SYNT:
        printf("Invalid syntax. Type 'zipme -h' for help.\n"); 
        break;
    case NO_DIRECTORY:
        printf("Wrong directory proceeded to './train.sh'.\n"); 
        break;
    case MULT_DIR:
        printf("Multiple directories passed to './train.sh'.\n"); 
        break;
    case FILE_OPEN_ERROR:
        printf("Could not open file.\n"); 
        break;
    case INDEX_LEN_ERROR:
        printf("Indexes are stored in 4 bytes, but sizeof(int) is %ld\n", sizeof(int)); 
        break;
    default:
        break;
    }
    printf(STD);
    exit(error);
}


void print_help()
{
    printf( "Usage: ./zipme -D [DICT_FILE] [OPTION] [FILE]...\n\n"
            "Default dictioanary is default.dict (if -D is omitted).\n\n"
            "Options:\n"
            "%5s-h\t\thelp\n"
            "%5s-d\t\tdecompress FILEs\n"
            "%5s-t\t\ttrain the dictionary on FILEs\n"

            "\nEXECUTE FILES ONLY FROM 'src' folder"
            "\nUse 'train.sh' script to train with all files from directory."
            "\n"
            "\nExamples:"
            "\n\t./zipme -t kit kat"
            "\n\t./zipme tic tac toe"
            "\n\t./zipme -d kit kat"
            "\n\t./zipme -D my_dict -d frkt.txt"
            "\n", " ", " ", " "
    );
}


