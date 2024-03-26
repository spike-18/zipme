#include "lib.cpp"

#ifndef DEBUG
    #define NDEBUG
#endif


int mode = COMPRESS;


int main(int argc, char* argv[])
{

    checkargs(argc, argv);
    mode = setmode(argv[1]);
    
    char* dict_name = NULL;

    switch (mode)
    {
        case COMPRESS:
            compress(dict_name, argv[1]);
            break;
        case DECOMPRESS:
            decompress(dict_name, argv[2]);
            break;
        case TRAIN:
            train(argc, argv);
            break;
        default:
            break;
    }

    return 0;
}