#include "lib.hpp"
#include "compress.cpp"
#include "decompress.cpp"
#include "train.cpp"



char* find_phrase(HashTable* ht, char* parent_index, char c)
{

    if (parent_index != NULL)
    {
        char str[MAX_STRING_LEN] = "";
        strcat(str, parent_index);
        char charStr[2] = {c, '\0'};
        strcat(str,charStr);
        
        char* ind = retrieve(ht, str);
        if (ind != NULL)
            return ind;
    }

    else
    {
        char charStr[2] = {c, '\0'};
        char* ind = retrieve(ht, charStr);
        if (ind != NULL)
            return ind;
    }
    
    return NULL;
}


void initHashTable(HashTable *ht) {
    for (int i = 0; i < MAX_DICT_LEN; ++i) {
        ht->table[i] = NULL;
    }
}


unsigned int hash(const char *key) {
    unsigned int hashVal = 0;
    while (*key) {
        hashVal = (hashVal << 5) + *key++;
    }
    return hashVal % MAX_DICT_LEN;
}


void insert(HashTable *ht, char *key) {
    unsigned int index = hash(key);
    //printf("insert %d, %s\n", index, key);
    ht->table[index] = key; // Store value directly in table
}


char *retrieve(HashTable *ht, const char *key) {
    unsigned int index = hash(key);
    char* entry = ht->table[index];
    // printf("%s - %s\n", key, entry);
    if (entry != NULL) {
        return entry;
    }
    return NULL; // Key not found
}

int get_index (const char* key)
{
    if (key == NULL)
        return -1;
    unsigned int index = hash(key);
    return index;
}



void read_dict (FILE* dictionary, HashTable* ht, char* buf)
{
    char c = (char) fgetc(dictionary);                                                 
    int i = 0;
    
    while (c != EOF)
    {
        buf[i] = c;
        c = (char) fgetc(dictionary);
        i++;
    }

    insert(ht, buf);
    for (int j = 1; j < i-1; j++)
        if (buf[j-1]=='\0')
            insert(ht, buf+j);
            
}


void print_dict(HashTable* ht)
{
    printf("______\n");
    for (int i = 0; i < MAX_DICT_LEN; i++)
        if (ht->table[i] != NULL)
        {
            printf("%d - %s\n", i, ht->table[i]);
        }
    printf("______\n");
    

}



int len_sort(const char *a, const char *b)
{
    return strlen(a) < strlen(b) ? -1 : 1;
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


