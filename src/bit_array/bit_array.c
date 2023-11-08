#include "bit_array.h"

#include <assert.h>
#include <stdlib.h>



BIT_ARRAY* bit_array_create(uint64_t nbits)
{
    BIT_ARRAY* bitarr = (BIT_ARRAY*)malloc(sizeof(BIT_ARRAY));

    if (NULL != bitarr)
    {
        bitarr->num_of_bits = (nbits / 8 + (nbits % 8 ? 1 : 0)) * 8;
        bitarr->buffer = (uint8_t*)malloc(bitarr->num_of_bits);
    }

    return bitarr;
}


void bit_array_free(BIT_ARRAY* bitarray)
{
    if (NULL != bitarray && NULL != bitarray->buffer)
    {
        free(bitarray->buffer);
    }

    free(bitarray);
}


struct Offset
{
    uint64_t byte;
    uint8_t bit_mask;
};
typedef struct Offset Offset;


Offset calc_offset(uint64_t offset_bits)
{
    Offset offset;
    offset.byte = offset_bits / 8;
    uint8_t bit = offset_bits % 8;
    offset.bit_mask = (128 >> bit);
    return offset;
}


void bit_array_set(BIT_ARRAY* bitarray, uint64_t offset_bits)
{
    assert(bitarray);
    assert(bitarray->num_of_bits > offset_bits);

    if (bitarray->num_of_bits > offset_bits)
    {
        Offset offset = calc_offset(offset_bits);
        uint8_t* p = bitarray->buffer + offset.byte;
        *p = *p | offset.bit_mask;
    }
}


uint8_t bit_array_get(BIT_ARRAY* bitarray, uint64_t offset_bits)
{
    assert(bitarray);
    assert(bitarray->num_of_bits > offset_bits);

    if (bitarray->num_of_bits > offset_bits)
    {
        Offset offset = calc_offset(offset_bits);
        uint8_t* p = bitarray->buffer + offset.byte;
        return *p & offset.bit_mask;
    }

    return 0;
}
