#include "lib.hpp"


int train(int argc, char* argv[])
{

    char* dir       = NULL;     
    char* dict_path = NULL;

    for (int i = 1; i < argc; i++)                                                              // Find the name of the directory
        if (strcmp(argv[i-1], "-r") == 0)                                                       // with training data files
            dir = argv[i];

    int file_piv = 1;                                                                           // Find the first argument with
    for (; file_piv < argc; file_piv++)                                                         // filename
        if (strcmp(argv[file_piv-1], "-t") == 0)
            break;


    if (dir != NULL)
        printf("Dictionary 'new.dict' will be created in '%s'.\n\n", dir);
    else
        printf("Dictionary 'default.dict' will be created current dir.\n\n");
    dict_path = concatenate(dir, "new.dict");


    FILE* dictionary = NULL;
    FILE* training_set = NULL;
    
    if ((dictionary = fopen(dict_path, "w+")) == NULL)
        report(FILE_OPEN_ERROR);
    free(dict_path);

    for (int i = file_piv; i < argc; i++)                                                       // Train the dictionary on each file from the argument list
    {        
        char* file_path = (dir == NULL) ? argv[file_piv] : concatenate(dir, argv[file_piv]);    // Concatenate to 'dir/filename' to open file correctly

        printf("FILE %d: %20s\t", i-file_piv, argv[i]);

        if ((training_set = fopen(file_path, "r")) != NULL)
        {
            free(file_path);

            int  index        = -1;                                                             // index == -1 is reserved for missing prefix in dictionary
            int  parent_index = -1;
            
            char c = (char) fgetc(training_set);                                                 
            

            while (c != EOF)
            {

                index = find_phrase(dictionary, parent_index, c);                               // Recursive search for the longest overlap between
                                                                                                // the inoput string and dictionaty values
                if (index != -1)
                    parent_index = index;
                else
                {
                    add_phrase(dictionary, parent_index, c);
                    parent_index = -1;
                }
                c = (char) fgetc(training_set);
            }

            fclose(training_set);

            printf(GRN);
            printf("complete\n");
            printf(STD);
        
        }
        else
        {
            printf(RED);
            printf("error\n");
            printf(STD);
        }

    }

    fclose(dictionary);

    return 0;
}



void add_phrase(FILE* dict, int parent_index, char c)
{

    long   pos  = ftell(dict);                                  // Save start file pointer position

    char*  line = NULL;
    size_t len  = 0;

    rewind(dict);                                               // Find prefix from the start of
    if (parent_index != -1)                                     // dictionary, NULL if not found
        for (int i = 0; i <= parent_index; i++)                 // values in dictionary are stored
            getdelim(&line, &len, '\0', dict);                  // in format 'data\n\0' - '\n' for
                                                                // better visibility

    
    fseek(dict, pos, SEEK_SET);                                 // Return back to where we've been
    
    if (line)                                                   // Put the prefix and the character                
        for (int i = 0; line[i+1] != '\0'; i++)                 // into the end of the dict. file
        {
            fputc(line[i], dict);
        }

    fputc(c, dict);
    fputc(10, dict);                                            // Entries are separated with '\n\0'
    fputc('\0', dict);
    fflush(dict);

    #ifdef DEBUG
    {
        printf(BLU);
        printf("Added: ");
        if (line)
            for (int i = 0; line[i+1] != '\0'; i++)
                printf("%c", line[i]);
        printf("%c\n", c);
        printf(STD);
    }
    #endif

    free(line);
}



char* concatenate(const char* dir, const char* name)
{
    if(dir)
    {
        char appendix[2];

        appendix[0] = '/';
        appendix[1] = '\0';

        char* _dir = strdup(dir);

        _dir = strcat(_dir, appendix);

        int i = 0;
        while (_dir[i] != '\0')
            i++;

        _dir[i-1] = (_dir[i-2] == '/') ? '\0' : _dir[i-1];          // If _dir contains '/' symbol, leave it, append otherwise
        
        return strcat(_dir, name);   
    
    }
    else
        return strdup("default.dict");
}