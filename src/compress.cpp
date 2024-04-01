#include "lib.hpp"


int compress(int argc, char* argv[])
{
    
    char* dict_path   = NULL;
    bool  custom_dict = NULL;

    for (int i = 1; i < argc; i++)                                                              // Find dictionary path
        if (strcmp(argv[i-1], "-D") == 0)
        {
            dict_path = argv[i];
            custom_dict = 1;
            break;
        }         


    if (!dict_path)
        dict_path = strdup("default.dict");

    int file_piv = 1;                                                                           // Find the first argument with
    for (int i = 0; file_piv+i < argc; i++)                                                     // filename
        if (strcmp(argv[file_piv+i], "-D") == 0)
        {
            file_piv+=i+2;            
            break;
        }

    
    FILE* dictionary = NULL;
    FILE* file = NULL;
    FILE* compressed_file = NULL;
    

    if ((dictionary = fopen(dict_path, "r")) == NULL)
        report(FILE_OPEN_ERROR);
    
    if (!custom_dict)
        free(dict_path);



    struct stat file_info = {};
    fstat( fileno(dictionary), &file_info );

    const size_t buf_len = file_info.st_size / sizeof(char);
    char*  buf     = (char*) calloc(buf_len, sizeof(char)); 


    HashTable ht;
    ht.table = (HashEntry**) calloc(MAX_DICT_LEN, sizeof(HashEntry*));
    
    
    initHashTable(&ht);
    read_dict(dictionary, &ht, buf);
    // print_dict(&ht);


    for (int i = file_piv; i < argc; i++)                                                       // Compress each file to file_#.com
    {        
        printf("FILE %d: %20s\t", i-file_piv, argv[i]);
        char file_name[100];
        sprintf(file_name, "file_%d.com", i-file_piv);


        if ((file = fopen(argv[i], "r")) != NULL && (compressed_file = fopen(file_name, "w+")) != NULL)
        {

            int    code = 0;
            char*  index        = NULL;                                                             // index == -1 is reserved for 
            char*  parent_index = NULL;                                                             // missing prefix in dictionary

            char c = (char) fgetc(file);    
            
            while (c != EOF)
            {
                index = find_phrase(&ht, parent_index, c);  
                
                if (index != NULL)
                    parent_index = index;
                else
                {
                    code = get_index(&ht, parent_index);
                    fwrite(&code, 1, INDEX_LN, compressed_file);
                    fwrite(&c, 1, sizeof(char), compressed_file);
                    parent_index = NULL;
                }
                c = (char) fgetc(file);
            }

            if (parent_index != NULL)
            {
                char escape = '\0';
                code = get_index(&ht, parent_index);
                fwrite(&code, 1, INDEX_LN, compressed_file);
                fwrite(&escape, 1, sizeof(char), compressed_file);
            }

            int eof_index = -2;                                                                 // index == -2 is reserved for
            fwrite(&eof_index, 1, INDEX_LN, compressed_file);                                   // end of the file
            fclose(compressed_file);
            fclose(file);

            printf(GRN);
            printf("comressed\n");
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
    free(buf);
    free_hash(&ht);

    return 0; 
}