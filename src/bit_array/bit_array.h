#ifndef __BIT_ARRAY__
#define __BIT_ARRAY__

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

struct BIT_ARRAY
{
    uint8_t* buffer;
    uint64_t num_of_bits;
};

typedef struct BIT_ARRAY BIT_ARRAY;

BIT_ARRAY* bit_array_create(uint64_t nbits);
void bit_array_free(BIT_ARRAY* bitarray);

void bit_array_set(BIT_ARRAY* bitarray, uint64_t offset);
void bit_array_unset(BIT_ARRAY* bitarray, uint64_t offset);
uint8_t bit_array_get(BIT_ARRAY* bitarray, uint64_t offset);

BIT_ARRAY* bit_array_or(BIT_ARRAY* dst, BIT_ARRAY* src1, BIT_ARRAY* src2);
int bit_array_eq(BIT_ARRAY* src1, BIT_ARRAY* src2);

uint64_t bit_array_hash(BIT_ARRAY* bitarray, uint64_t mix);

#ifdef __cplusplus
}
#endif

#endif //__BIT_ARRAY__
