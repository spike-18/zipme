#include "lib.hpp"


int decompress(char* dict_name, int argc, char* argv[])
{

    FILE* dictionary = NULL;
    FILE* compressed_file = NULL;
    FILE* decompressed_file = NULL;

    int file_piv = 1;                                                                           // Find the first argument with
    for (; file_piv < argc-1; file_piv++)                                                         // filename
        if (strcmp(argv[file_piv], "-d") == 0)
        {
            file_piv++;            
            break;
        }

    
    if ((dictionary = fopen("../dict/new.dict", "r")) == NULL)
        assert(dictionary);

    
    if ((compressed_file = fopen(argv[file_piv], "r")) != NULL && (decompressed_file = fopen("../data/decompressed.txt", "w+")) != NULL)
    {

        int  parent_index = -1;
        char c = 0;

        while (1)
        {
            fread(&parent_index, 1, INDEX_LN, compressed_file);
            
            if (parent_index == -2)
                break;

            fread(&c, 1, sizeof(char), compressed_file);

            write_from_dict(decompressed_file, dictionary, parent_index, c);
        }

        fclose(compressed_file);
        fclose(decompressed_file);

    }


    fclose(dictionary);

    return 0; 
}


int write_from_dict(FILE* file, FILE* dict, int parent_index, char c)
{

    char*  line = NULL;
    size_t  buf  = 0;

    rewind(dict);                                               // Find prefix from the start of
    if (parent_index != -1)                                     // dictionary, NULL if not found
        for (int i = 0; i <= parent_index; i++)
            getdelim(&line, &buf, '\0', dict);
    
    if (line)                                                    // Put the prefix and the character                
        for (int i = 0; line[i+1] != '\0'; i++)                  // into the end of the dict. file
            fputc(line[i], file);
        
    if(c) fputc(c, file);


    free(line);

    return 0;
}