#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stddef.h>

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
    NO_INPUT_FILE   = -1,
    FILE_OPEN_ERROR = -2,
    NO_DICT         = -3,
    INV_SYNT        = -4,
    NO_DIRECTORY    = -5,
    MULT_DIR        = -6,
    INDEX_LEN_ERROR = -7,
    DICT_LEN_EXIT   = -8,
};


typedef struct HashEntry {
    char *value;
    int uindex;                 // Unique index globally
    struct HashEntry *next;     // For handling collisions with chaining
} HashEntry;


typedef struct {
    HashEntry **table;
} HashTable;




typedef int (*CompareFunction)(const char *, const char *);


void            print_help          (void);
void            report              (int error);
void            checkargs           (int argc, char* argv[]);
int             setmode             (int argc, char* argv[]);
int             train               (int argc, char* argv[]);
int             compress            (int argc, char* argv[]);
int             decompress          (int argc, char* argv[]);
char*           concatenate         (const char* dir, const char* name);
void            add_phrase          (char** buf_new, HashTable* ht, char* parent_index, char c);
char*           find_phrase         (HashTable* ht, char* parent_index, char c);
int             write_from_dict     (FILE* file, HashTable* ht, int parent_index, char c);
char*           print_to_buf        (char* buf_new, char* str);
void            save_dict           (FILE* dictionary, HashTable* ht);
void            print_dict          (HashTable* ht);
void            read_dict           (FILE* dictionary, HashTable* ht, char* buf);
int             len_sort            (const char *a, const char *b);
unsigned int    hash                (const char *key);
void            initHashTable       (HashTable *ht);
void            insert              (HashTable *ht, char *key);
char*           retrieve            (HashTable *ht, const char *key);
int             get_index           (HashTable* ht, const char* key);
int             replace             (HashTable* ht, char* key);
uint32_t        murmurhash3         (const char *key, uint32_t seed);
void            insert_entry        (HashTable* ht, int index, HashEntry* entry);
void            free_hash           (HashTable* ht);
uint32_t fnv1_hash(const char *key);
