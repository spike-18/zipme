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

#define STD "\033[1;0m"
#define BLK "\u001b[30;1m"
#define RED "\u001b[31;1m"
#define GRN "\u001b[32;1m"
#define YEL "\u001b[33;1m"
#define BLU "\u001b[34;1m"
#define MAG "\u001b[35;1m"
#define CYN "\u001b[36;1m"
#define WHT "\u001b[37;1m"

#define INDEX_LN        4              // number of bytes to store index in compressed file
#define MAX_DICT_LEN    INT_MAX        // max number of values in dictionary


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




void    print_help      (void);
void    report          (int error);
void    checkargs       (int argc, char* argv[]);
int     setmode         (int argc, char* argv[]);
int     train           (int argc, char* argv[]);
int     compress        (int argc, char* argv[]);
int     decompress      (int argc, char* argv[]);
char*   concatenate     (const char* dir, const char* name);
void    add_phrase      (FILE* dict, int parent_index, char c);
int     find_phrase     (FILE* dict, int parent_index, char c);
int     write_from_dict (FILE* file, FILE* dict, int parent_index, char c);
     