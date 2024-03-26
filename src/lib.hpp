#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


enum MODES {
    COMPRESS    = 0,
    DECOMPRESS  = 1,
    TRAIN       = 2
};

enum ERRORS {
    NO_INPUT_FILE  = 1,
    FILE_NOT_FOUND = 2,
    MULT_COMPRESS  = 3,
    NO_DICT        = 4,
    INV_SYNT       = 5,
};

void   print_help     (void);
void   report         (int error);
char*  checkargs      (int argc, char* argv[]);
int    setmode        (int argc, char* argv[]);
int    train          (int argc, char* argv[]);
int    decompress     (char* dict_name, int argc, char* argv[]);
int    compress       (char* dict_name, int argc, char* argv[]);