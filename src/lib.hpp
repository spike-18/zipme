#pragma once

#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/stat.h>


#define STD "\033[1;0m"
#define BLK "\u001b[30;1m"
#define RED "\u001b[31;1m"
#define GRN "\u001b[32;1m"
#define YEL "\u001b[33;1m"
#define BLU "\u001b[34;1m"
#define MAG "\u001b[35;1m"
#define CYN "\u001b[36;1m"
#define WHT "\u001b[37;1m"


#define INDEX_LN            4              // number of bytes to store index in compressed file
#define MAX_DICT_LEN        1000000        // max number of values in dictionary
#define INIT_BUF_LEN        INT_MAX        // initial buffer length to store dictionary
#define MAX_STRING_LEN      100


enum MODES {
    COMPRESS    = 0,
    DECOMPRESS  = 1,
    TRAIN       = 2
};

enum ERRORS {
    NO_INPUT_FILE,
    FILE_OPEN_ERROR,
    NO_DICT,
    INV_SYNT,
    NO_DIRECTORY,
    MULT_DIR,
    INDEX_LEN_ERROR,
    DICT_LEN_EXIT
};



typedef struct HashEntry {
    char *value;
    int uindex;                 // Unique index globally
    struct HashEntry *next;     // For handling collisions with chaining
} HashEntry;

typedef struct {
    HashEntry **table;
} HashTable;



int          train           (int argc, char* argv[]);
int          setmode         (int argc, char* argv[]);
int          compress        (int argc, char* argv[]);
int          decompress      (int argc, char* argv[]);
int          replace         (HashTable* ht, char* key);
int          get_index       (HashTable* ht, const char* key);
char*        print_to_buf    (char* buf_new, char* str);
char*        retrieve        (HashTable *ht, const char *key);
char*        concatenate     (const char* dir, const char* name);
char*        find_phrase     (HashTable* ht, char* parent_index, char c);
void         print_help      (void);
void         report          (int error);
void         print_dict      (HashTable* ht);
void         free_hash       (HashTable* ht);
void         initHashTable   (HashTable *ht);
void         checkargs       (int argc, char* argv[]);
void         insert          (HashTable *ht, char *key);
void         save_dict       (FILE* dictionary, HashTable* ht);
void         read_dict       (FILE* dictionary, HashTable* ht, char* buf);
void         add_phrase      (char** buf_new, HashTable* ht, char* parent_index, char c);
void         insert_entry    (HashTable* ht, int index, HashEntry* entry);
uint32_t     fnv1_hash       (const char *key);
unsigned int hash            (const char *key);
void         code            (FILE* file, int index);
int          decode          (FILE* file);