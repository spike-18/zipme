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

    int dict_len = 0;  
    char** DS = (char**) calloc(MAX_DICT_LEN, sizeof(char*));
    read_dict(dictionary, DS, &dict_len, buf);
    // print_dict(DS, dict_len);


    for (int i = file_piv; i < argc; i++)                                                       // Compress each file to file_#.com
    {        
        printf("FILE %d: %20s\t", i-file_piv, argv[i]);
        char file_name[100];
        sprintf(file_name, "file_%d.com", i-file_piv);


        if ((file = fopen(argv[i], "r")) != NULL && (compressed_file = fopen(file_name, "w+")) != NULL)
        {


            int charnum = 0; 
            struct stat fs;
            fstat(fileno(file), &fs);

            int  index        = -1;                                                             // index == -1 is reserved for 
            int  parent_index = -1;                                                             // missing prefix in dictionary

            char c = (char) fgetc(file);    
            
            while (c != EOF)
            {


                /// DEBUG CODE
                charnum++;
                if(charnum%10000==0)
                {
                    printf("\r%d / %ld\n", charnum, fs.st_size);
                }
                ///


                index = find_phrase(DS, dict_len, parent_index, c);  
                
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
    free(DS);

    return 0; 
}