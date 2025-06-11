#include "./fnv_hash.h"

#define FNV_OFFSET_32 2166136261U
#define FNV_PRIME_32 16777619U

uint32_t fnv1a_32(const char *s) {
    uint32_t hash = FNV_OFFSET_32;
    while (*s) {
        hash ^= (uint32_t)*s++;
        hash *= FNV_PRIME_32;
    }
    return hash;
}