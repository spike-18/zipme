#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

};


void checkargs (int argc, char* argv[]);
void report    (int error);