#include "lib.hpp"

int train(int argc, char* argv[])
{
    printf("Training with files:\n\n");
    int i = 0; 
    int j = 0;
    for (; i < argc; i++)
        if(strcmp(argv[i], "-t")==0)
            break;
    for (i++;i < argc; i++, j++)
        printf("FILE %d: %s\n", j, argv[i]);
    
    return 0;
}