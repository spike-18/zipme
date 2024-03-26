#include "lib.hpp"

int train(int argc, char* argv[])
{
    for (int i = 2; i < argc; i++)
    {
        printf("File %d: %s\n", i-2, argv[i]);
    }
    
    return 0;
}