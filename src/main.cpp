#include "lib.cpp"

#ifndef DEBUG
    #define NDEBUG
#endif


int mode = COMPRESS;


int main(int argc, char* argv[])
{

    checkargs(argc, argv);
    mode = setmode(argc, argv);

    if (sizeof(int) != INDEX_LN)
    {
        printf(RED);
        printf("sizof(int) %ld != %d (INDEX_LN)\n", sizeof(int), INDEX_LN);
        report(INDEX_LEN_ERROR);
        printf(STD);
    }

    #ifdef DEBUG
    printf("Dictionary: %s\n", dict_name);
    printf("Mode [ (%d)-comp, (%d)-decomp, (%d)-train ]: %d\n\n", COMPRESS, DECOMPRESS, TRAIN, mode);
    #endif
    
    switch (mode)
    {
        case COMPRESS:
            compress(argc, argv);
            break;
        case DECOMPRESS:
            decompress(argc, argv);
            break;
        case TRAIN:
            train(argc, argv);
            break;
        default:
            break;
    }

    return 0;
}