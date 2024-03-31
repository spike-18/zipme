#include "lib.hpp"
#include "compress.cpp"
#include "decompress.cpp"
#include "train.cpp"



int find_phrase(char** dict, int dict_len, int parent_index, char c)
{

    if (parent_index!=-1)
    {
        int parent_len = 0;
        for (size_t i = 0; dict[parent_index][i] != '\0'; i++)
            parent_len++;

        for (int i = 0; i < dict_len; i++)
        {
            if (strncmp(dict[i], dict[parent_index], parent_len) == 0)
                if(dict[i][parent_len] == c)
                    return i;
        }
    }
    else
    {
        for (int i = 0; i < dict_len; i++)
            if (dict[i][0] == c)
                return i;
    }
    
    return -1;
}


void read_dict (FILE* dictionary, char** DS, int* dict_len, char* buf)
{
    DS[0] = buf;
    *dict_len += 1;

    char c = (char) fgetc(dictionary);                                                 
    buf[0] = c;
    int i = 1;

    c = (char) fgetc(dictionary);
    while (c != EOF)
    {
        buf[i] = c;

        if (buf[i-1] == '\0')
        {
            DS[*dict_len] = buf+i;
            *dict_len += 1;
        }
        i++;

        c = (char) fgetc(dictionary);
    }

}


void print_dict(char** DS, int dict_len)
{
    for (int i = 0; i < dict_len; i++)
        printf("%s",DS[i]);
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
