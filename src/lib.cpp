#include "lib.hpp"


void checkargs(int argc, char* argv[])
{
    if (argc < 2)
        report(NO_INPUT_FILE);
    else if (argc > 2 && strcmp(argv[1], "--train"))
        report(MULT_COMPRESS);
}


void report(int error)
{
    switch (error)
    {
        case NO_INPUT_FILE:
            assert(!"No input file to compress."); 
            break;
        case MULT_COMPRESS:
            assert(!"Cannot compress/decompress multiple files yet."); 
            break;
        default:
            break;
    }
}