#include "lib.hpp"
#include "compress.cpp"
#include "decompress.cpp"
#include "train.cpp"



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


char* checkargs(int argc, char* argv[])
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
            else if (argc-1 - i >= 2)
                report(MULT_COMPRESS);
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

    // Finds the dictionary argument and return its name
    for (int i = 1; i < argc-1; ++i)
        if (strcmp(argv[i], "-D") == 0)
            return argv[i+1];
    
    return NULL;
}


void report(int error)
{
    printf(RED);
    switch (error)
    {
    case NO_INPUT_FILE:
        printf("No input file(s).\n"); 
        break;
    case MULT_COMPRESS:
        printf("Cannot compress/decompress multiple files yet.\n"); 
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
