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
    {
        printf("Dictionary 'new.dict' will be created in '%s'.\n\n", dir);
        dict_path = concatenate(dir, "new.dict");
    }
    else
    {
        printf("Dictionary 'default.dict' will be created current dir.\n\n");
        dict_path = strdup("default.dict");
    }


    FILE* dictionary = NULL;
    FILE* training_set = NULL;
    
    if ((dictionary = fopen(dict_path, "w+")) == NULL)
        report(FILE_OPEN_ERROR);
    free(dict_path);


    size_t buf_len = INIT_BUF_LEN;
    char*  buf     = (char*) calloc(buf_len, sizeof(char));
    char*  buf_new = buf;

    int dict_len = 0;                                                                  // store number of rows in dict
    char** DS = (char**) calloc(MAX_DICT_LEN, sizeof(char*));


    for (int i = file_piv; i < argc; i++)                                                       // Train the dictionary on each file from the argument list
    {        

        char* file_path = (dir == NULL) ? argv[file_piv] : concatenate(dir, argv[file_piv]);    // Concatenate to 'dir/filename' to open file correctly

        printf("FILE %d: %20s\t", i-file_piv, argv[i]);

        if ((training_set = fopen(file_path, "r")) != NULL)
        {    
            
            int charnum = 0; 
            struct stat fs;
            fstat(fileno(training_set), &fs);

            
            if(dir) free(file_path);

            int  index        = -1;                                                             // index == -1 is reserved for missing prefix in dictionary
            int  parent_index = -1;
            
            char c = (char) fgetc(training_set);                                                 
            

            while (c != EOF)
            {
                /// DEBUG CODE
                charnum++;
                if(charnum%10000==0)
                {
                    printf("%d:\t %d / %ld\n", dict_len, charnum, fs.st_size);
                }
                ///


                index = find_phrase(DS, dict_len, parent_index, c);                               // Recursive search for the longest overlap between
                                                                                                // the inoput string and dictionaty values
                if (index != -1)
                    parent_index = index;
                else
                {
                    if (dict_len == MAX_DICT_LEN)
                    {
                        printf("Dictionary length reached its limit.\n");
                        save_dict(dictionary, DS, dict_len);
                        exit(DICT_LEN_EXIT);
                    }

                    add_phrase(&buf_new, DS, &dict_len, parent_index, c);
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

    save_dict(dictionary, DS, dict_len);
    // print_dict(DS, dict_len);
    fclose(dictionary);
    free(buf);
    free(DS);

    return 0;
}



void add_phrase(char** buf_new, char** dict, int* dict_len, int parent_index, char c)
{

    dict[*dict_len] = *buf_new;
    *dict_len += 1;

                                                                // Find prefix from the start of
    if (parent_index != -1)                                     // dictionary, NULL if not found
        *buf_new = print_to_buf(*buf_new, dict[parent_index]);

    (*buf_new)[0] = c;
    (*buf_new)[1] = '\0';
    *buf_new = *buf_new+2;

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

char* print_to_buf(char* buf_new, char* str)
{
    size_t i = 0;
    for (; str[i] != '\0'; i++)
        buf_new[i] = str[i];

    return buf_new+i;
}

void save_dict(FILE* dictionary, char** DS, int dict_len)
{
    for (int i = 0; i < dict_len; i++)
    {
        fputs(DS[i], dictionary);
        fputc('\0', dictionary);
    }
}
