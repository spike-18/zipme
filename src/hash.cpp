#include "lib.hpp"


#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U



uint32_t fnv1_hash(const char *key) {
    size_t len = strlen(key);
    const unsigned char *bytes = (const unsigned char *)key;
    uint32_t hash = FNV_OFFSET_32;

    for (size_t i = 0; i < len; ++i) {
        hash ^= (uint32_t)bytes[i];
        hash *= FNV_PRIME_32;
    }

    return hash % MAX_DICT_LEN;
}

