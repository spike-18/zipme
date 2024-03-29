#include "lib.hpp"


int train(int argc, char* argv[])
{

    char* dir = NULL;                                                                           

    for (int i = 1; i < argc; i++)                                                              // Find the name of the directory
        if (strcmp(argv[i-1], "-r") == 0)                                                       // with training data files
            dir = argv[i];

    int file_piv = 1;                                                                           // Find the first argument with
    for (; file_piv < argc; file_piv++)                                                         // filename
        if (strcmp(argv[file_piv-1], "-t") == 0)
            break;


    FILE* dictionary = NULL;
    FILE* training_set = NULL;
    
    if ((dictionary = fopen("../dict/new.dict", "w+")) == NULL)
        assert(dictionary);


    for (int i = file_piv; i < argc; i++)                                                       // Train the dictionary on each file from the argument list
    {        
        char* file_path = (dir == NULL) ? argv[file_piv] : concatenate(dir, argv[file_piv]);    // Concatenate dir/filename to open file correctly

        if ((training_set = fopen(file_path, "r")) != NULL)
        {

            printf("FILE %d: %20s\t", i-file_piv, argv[i]);

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
    }

    fclose(dictionary);

    return 0;
}



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
            parent_len = getline(&parent_line, &buf_size, dict);
    buf_size = 0;
        
    rewind(dict);                                                                   // Start compairing prefix with dictionary
    len = getline(&line, &buf_size, dict);                                          // entries from start of the file
    
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

        if ( strncmp(line, parent_line, parent_len ? parent_len-1 : 0) == 0 )       // If prefix is found in dictionary
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
            
            if (len > parent_len && line[parent_len-1] == c)                        // see if the next characters after
                return index;                                                       // the end of prefix are equal
            
            #ifdef DEBUG
            {
                if (len > parent_len)
                printf("Need: %c, Found: %c\n", c, line[parent_len-1]);
            }
            #endif

        }

        index++;
        len = getline(&line, &buf_size, dict);                                      // compare with next entry
    }

    free(line);
    free(parent_line);

    return -1;
}



void add_phrase(FILE* dict, int parent_index, char c)
{

    long   pos  = ftell(dict);                                  // Save start file pointer position

    char*  line = NULL;
    size_t len  = 0;

    rewind(dict);                                               // Find prefix from the start of
    if (parent_index != -1)                                     // dictionary, NULL if not found
        for (int i = 0; i <= parent_index; i++)
            getline(&line, &len, dict);
    
    fseek(dict, pos, SEEK_SET);                                 // Return back to where we've been
    
    if (line)                                                    // Put the prefix and the character                
        for (int i = 0; line[i] != '\n'; i++)                   // into the end of the dict. file
            fputc(line[i], dict);
        
    fputc(c, dict);
    fputc(10, dict);                                            // Entries are separated with '\n'
    fflush(dict);

    #ifdef DEBUG
    {
        printf(BLU);
        printf("Added: ");
        if (line)
            for (int i = 0; line[i]!='\n'; i++)
                printf("%c", line[i]);
        printf("%c\n", c);
        printf(STD);
    }
    #endif

    free(line);
}



char* concatenate(const char* dir, const char* name)
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