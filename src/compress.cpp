#include "lib.hpp"


int compress(char* dict_name, int argc, char* argv[])
{
    
    FILE* dictionary = NULL;
    FILE* file = NULL;
    FILE* compressed_file = NULL;

    int file_piv = 1;                                                                           // Find the first argument with
    for (; file_piv < argc-1; file_piv++)                                                         // filename
        if (strcmp(argv[file_piv], "-D") == 0)
        {
            file_piv++;            
            break;
        }

    
    if ((dictionary = fopen("../dict/new.dict", "r")) == NULL)
        assert(dictionary);

    
    if ((file = fopen(argv[file_piv], "r")) != NULL && (compressed_file = fopen("../data/compressed.txt", "w+")) != NULL)
    {

        int  index        = -1;                                                             // index == -1 is reserved for missing prefix in dictionary
        int  parent_index = -1;

        char c = (char) fgetc(file);    
        
        while (c != EOF)
        {
            index = find_phrase(dictionary, parent_index, c);  
            
            if (index != -1)
                parent_index = index;
            else
            {
                fwrite(&parent_index, 1, INDEX_LN, compressed_file);
                fwrite(&c, 1, sizeof(char), compressed_file);
                parent_index = -1;
            }
            c = (char) fgetc(file);
        }

        if (parent_index != -1)
        {
            char escape = '\0';
            fwrite(&parent_index, 1, INDEX_LN, compressed_file);
            fwrite(&escape, 1, sizeof(char), compressed_file);
        }

        int eof_index = -2;
        fwrite(&eof_index, 1, INDEX_LN, compressed_file);
        fclose(compressed_file);
        fclose(file);

    }


    fclose(dictionary);

    return 0; 
}