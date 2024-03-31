#include "lib.hpp"


int decompress(int argc, char* argv[])
{

    char* dict_path = NULL;
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
    for (; file_piv < argc-1; file_piv++)                                                       // filename
        if (strcmp(argv[file_piv], "-d") == 0)
        {
            file_piv++;            
            break;
        }

    
    FILE* dictionary = NULL;
    FILE* compressed_file = NULL;
    FILE* decompressed_file = NULL;
    
    if ((dictionary = fopen(dict_path, "r")) == NULL)
        report(FILE_OPEN_ERROR);
   
    if (!custom_dict)
        free(dict_path);


    struct stat file_info = {};
    fstat( fileno(dictionary), &file_info );

    const size_t buf_len = file_info.st_size / sizeof(char);
    char*  buf     = (char*) calloc(buf_len, sizeof(char)); 

    HashTable ht;
    ht.table = (char**) calloc(MAX_DICT_LEN, sizeof(char*));
    initHashTable(&ht);
    read_dict(dictionary, &ht, buf);
    // print_dict(&ht);

    for (int i = file_piv; i < argc; i++)                                                       // Decompress each file to file_#.dec
    {        
        printf("FILE %d: %20s\t", i-file_piv, argv[i]);
        char file_name[100];
        sprintf(file_name, "file_%d.dec", i-file_piv);
    
        if ((compressed_file = fopen(argv[i], "r")) != NULL && (decompressed_file = fopen(file_name, "w+")) != NULL)
        {

            int parent_index = -1;
            char c = 0;

            while (1)
            {
                fread(&parent_index, 1, INDEX_LN, compressed_file);
                
                if (parent_index == -2)
                    break;

                fread(&c, 1, sizeof(char), compressed_file);

                write_from_dict(decompressed_file, &ht, parent_index, c);
            }

            fclose(compressed_file);
            fclose(decompressed_file);

            printf(GRN);
            printf("decompressed\n");
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
    free(ht.table);

    return 0; 
}


int write_from_dict(FILE* file, HashTable* ht, int parent_index, char c)
{
    if (parent_index != -1)                                                                     // dictionary
         fputs(ht->table[parent_index], file);
 
    if(c) fputc(c, file);

    return 0;
}