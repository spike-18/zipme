#include "lib.hpp"


int train(int argc, char* argv[])
{
    char* dir = NULL;
    for (int i = 1; i < argc; i++)
        if(strcmp(argv[i-1], "-r")==0)
            dir = argv[i];

    int file_piv = 1;
    for (; file_piv < argc; file_piv++)
        if(strcmp(argv[file_piv-1], "-t")==0)
            break;

    FILE* dictionary = fopen("../dict/new.dict", "w+");
    FILE* training_set = NULL;

    for (int i = file_piv; i < argc; i++)
    {        
        char* file_path = (dir == NULL) ? argv[file_piv] : concatenate(dir, argv[file_piv]);

        if ((training_set = fopen(file_path, "r")) != NULL)
        {
            
            printf("FILE %d: %20s\t", i-file_piv, argv[i]);

            long index        = -1;
            long parent_index = -1;
            char c = (char) fgetc(training_set);
            
            while (c != EOF)
            {
                index = find_phrase(dictionary, parent_index, c);

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

long find_phrase(FILE* dict, long parent_index, char c)
{

    unsigned long index = 0;

    char* line = NULL;
    char* parent_line = NULL;
    ssize_t len = 0;
    ssize_t parent_len = 0;
    size_t buf_size = 0;
    
    rewind(dict);
    if(parent_index != -1)
        for (long i = 0; i <= parent_index; i++)
            parent_len = getline(&parent_line, &buf_size, dict);
    buf_size = 0;
        
    rewind(dict);
    len = getline(&line, &buf_size, dict);
    
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
            if(parent_line) for (int i = 0; parent_line[i]!='\n'; i++)
                printf("%c", parent_line[i]);
            if(line) printf(" Dict: ");
            for (int i = 0; line[i]!='\n'; i++)
                printf("%c", line[i]);
            printf("\n");
        }
        #endif

        if (strncmp(line, parent_line, parent_len ? parent_len-1 : 0) == 0)
        {

            #ifdef DEBUG
            {
                printf(GRN);
                printf("(PREFIX MATCH)\n");
                printf(STD);
            }
            #endif

            if (parent_index == -1 && line[0] == c)
                return index;
            if (len > parent_len && line[parent_len-1] == c)
                return index;
            
            #ifdef DEBUG
            {
                if (len > parent_len)
                printf("Need: %c, Found: %c\n", c, line[parent_len-1]);
            }
            #endif

        }

        index++;
        len = getline(&line, &buf_size, dict);
    }

    free(line);
    free(parent_line);

    return -1;
}

void add_phrase(FILE* dict, long parent_index, char c)
{

    long   pos  = ftell(dict);
    char*  line = NULL;
    size_t len  = 0;

    rewind(dict);
    if (parent_index != -1)
        for (long i = 0; i <= parent_index; i++)
            getline(&line, &len, dict);
    
    fseek(dict, pos, SEEK_SET);
    if(line)
        for (int i = 0; line[i]!='\n'; i++)
            fputc(line[i], dict);
        
    fputc(c, dict);
    fputc(10, dict);
    fflush(dict);

    #ifdef DEBUG
    {
        printf(BLU);
        printf("Added: ");
        if(line)
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
    _dir[i-1] = _dir[i-2]=='/' ? '\0' : _dir[i-1];

    return strcat(_dir, name);   
}