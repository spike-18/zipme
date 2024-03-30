#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

#define STD "\033[1;0m"
#define BLK "\u001b[30;1m"
#define RED "\u001b[31;1m"
#define GRN "\u001b[32;1m"
#define YEL "\u001b[33;1m"
#define BLU "\u001b[34;1m"
#define MAG "\u001b[35;1m"
#define CYN "\u001b[36;1m"
#define WHT "\u001b[37;1m"

#define INDEX_LN 4  // number of bytes to store index in compressed file



enum MODES {
    COMPRESS    = 0,
    DECOMPRESS  = 1,
    TRAIN       = 2
};


enum ERRORS {
    NO_INPUT_FILE   = -1,
    FILE_OPEN_ERROR = -2,
    MULT_COMPRESS   = -3,
    NO_DICT         = -4,
    INV_SYNT        = -5,
    NO_DIRECTORY    = -6,
    MULT_DIR        = -7,
};




void    print_help      (void);
void    report          (int error);
int     setmode         (int argc, char* argv[]);
int     train           (int argc, char* argv[]);
char*   checkargs       (int argc, char* argv[]);
char*   concatenate     (const char* dir, const char* name);
void    add_phrase      (FILE* dict, int parent_index, char c);
int     find_phrase     (FILE* dict, int parent_index, char c);
int     compress        (char* dict_name, int argc, char* argv[]);
int     decompress      (char* dict_name, int argc, char* argv[]);
int     write_from_dict (FILE* file, FILE* dict, int parent_index, char c);
     