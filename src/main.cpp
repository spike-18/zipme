#include "lib.cpp"

#ifndef DEBUG
    #define NDEBUG
#endif


int mode = COMPRESS;

int main(int argc, char* argv[])
{

    char* dict_name = checkargs(argc, argv);
    mode = setmode(argc, argv);

    #ifdef DEBUG
    printf("Dictionary: %s\n", dict_name);
    printf("Mode [ (%d)-comp, (%d)-decomp, (%d)-train ]: %d\n\n", COMPRESS, DECOMPRESS, TRAIN, mode);
    #endif
    
    switch (mode)
    {
        case COMPRESS:
            compress(dict_name, argc, argv);
            break;
        case DECOMPRESS:
            decompress(dict_name, argc, argv);
            break;
        case TRAIN:
            train(argc, argv);
            break;
        default:
            break;
    }

    return 0;
}