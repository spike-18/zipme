#include "lib.hpp"
#include "compress.cpp"
#include "decompress.cpp"
#include "train.cpp"



int find_phrase(FILE* dict, int parent_index, char c)
{

    int index = 0;

    char* line        = NULL;
    char* parent_line = NULL;
    ssize_t len        = 0;
    ssize_t parent_len = 0;
    size_t  buf_size   = 0;
    
    rewind(dict);                                                                   // Find prefix from the start of dictionary
    if (parent_index != -1)
        for (int i = 0; i <= parent_index; i++)
            parent_len = getdelim(&parent_line, &buf_size, '\0', dict);
    buf_size = 0;
        
    rewind(dict);                                                                   // Start compairing prefix with dictionary
    len = getdelim(&line, &buf_size, '\0', dict);                                          // entries from start of the file
    
    #ifdef DEBUG
    {
        printf(RED);
        printf("CHAR %c\n", c);
        printf(STD);
    }
    #endif

    while (len != -1)
    {

        #ifdef DEBUG
        {
            printf("Prefix: ");
            if (parent_line) for (int i = 0; parent_line[i]!='\n'; i++)
                printf("%c", parent_line[i]);
            if (line) printf(" Dict: ");
            for (int i = 0; line[i]!='\n'; i++)
                printf("%c", line[i]);
            printf("\n");
        }
        #endif

        if ( strncmp(line, parent_line, parent_len ? parent_len-2 : 0) == 0 )       // If prefix is found in dictionary
        {                                                                           // or if it's empty

            #ifdef DEBUG
            {
                printf(GRN);
                printf("(PREFIX MATCH)\n");
                printf(STD);
            }
            #endif

            if (parent_index == -1 && line[0] == c)                                 // see if dictionary contains character
                return index;                                                       // (if prefix is empty)
            
            if (len > parent_len && line[parent_len-2] == c)                        // see if the next characters after
                return index;                                                       // the end of prefix are equal
            
            #ifdef DEBUG
            {
                if (len > parent_len)
                printf("Need: %c, Found: %c\n", c, line[parent_len-1]);
            }
            #endif

        }

        index++;
        len = getdelim(&line, &buf_size, '\0', dict);                                          // entries from start of the file
    }

    free(line);
    free(parent_line);

    return -1;
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
