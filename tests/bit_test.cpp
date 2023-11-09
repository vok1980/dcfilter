#include "gtest/gtest.h"
#include "gtest/gtest-param-test.h"

#include "../src/bit_array/bit_array.h"


#define BIT0 (1 << 7)
#define BIT1 (1 << 6)
#define BIT2 (1 << 5)
#define BIT3 (1 << 4)
#define BIT4 (1 << 3)
#define BIT5 (1 << 2)
#define BIT6 (1 << 1)
#define BIT7 (1 << 0)


class ArraySizeParameterizedTestFixture :
    public ::testing::TestWithParam<std::pair<int, int>>
{
};


TEST_P(ArraySizeParameterizedTestFixture, CheckArraySize)
{
    std::pair<int, int> data = GetParam();

    BIT_ARRAY* array = bit_array_create(data.first);
    EXPECT_TRUE(NULL != array);
    EXPECT_EQ(array->num_of_bits, data.second);
    bit_array_free(array);
}


INSTANTIATE_TEST_SUITE_P(ArraySizeTests,
                         ArraySizeParameterizedTestFixture,
                         testing::Values(
                            std::make_pair<int, int>(0, 0),
                            std::make_pair<int, int>(1, 8),
                            std::make_pair<int, int>(2, 8),
                            std::make_pair<int, int>(3, 8),
                            std::make_pair<int, int>(4, 8),
                            std::make_pair<int, int>(5, 8),
                            std::make_pair<int, int>(6, 8),
                            std::make_pair<int, int>(7, 8),
                            std::make_pair<int, int>(8, 8),
                            std::make_pair<int, int>(9, 16),
                            std::make_pair<int, int>(10, 16),
                            std::make_pair<int, int>(11, 16),
                            std::make_pair<int, int>(12, 16),
                            std::make_pair<int, int>(13, 16),
                            std::make_pair<int, int>(14, 16),
                            std::make_pair<int, int>(15, 16),
                            std::make_pair<int, int>(16, 16),
                            std::make_pair<int, int>(17, 24),
                            std::make_pair<int, int>(18, 24),
                            std::make_pair<int, int>(19, 24),
                            std::make_pair<int, int>(20, 24),
                            std::make_pair<int, int>(21, 24),
                            std::make_pair<int, int>(22, 24),
                            std::make_pair<int, int>(23, 24),
                            std::make_pair<int, int>(24, 24)
                        ));


TEST(BitArray, CheckBitSet)
{
    BIT_ARRAY* array = bit_array_create(9);
    EXPECT_TRUE(NULL != array);
    EXPECT_EQ(array->num_of_bits, 16);

    bit_array_set(array, 7);
    EXPECT_EQ(*array->buffer, 1);
    EXPECT_EQ(*(array->buffer + 1), 0);

    bit_array_set(array, 9);
    EXPECT_EQ(*array->buffer, 1);
    EXPECT_EQ(*(array->buffer + 1), 64);

    bit_array_set(array, 15);
    EXPECT_EQ(*array->buffer, 1);
    EXPECT_EQ(*(array->buffer + 1), 65);

    bit_array_set(array, 0);
    EXPECT_EQ(*array->buffer, 129);
    EXPECT_EQ(*(array->buffer + 1), 65);

    ASSERT_DEBUG_DEATH(bit_array_set(array, 16), "");

    bit_array_free(array);
}


TEST(BitArray, CheckBitGet)
{
    BIT_ARRAY* array = bit_array_create(14);
    EXPECT_TRUE(NULL != array);
    EXPECT_EQ(array->num_of_bits, 16);

    bit_array_set(array, 7);
    EXPECT_TRUE(bit_array_get(array, 7));
    EXPECT_FALSE(bit_array_get(array, 6));
    EXPECT_FALSE(bit_array_get(array, 8));

    bit_array_set(array, 8);
    EXPECT_TRUE(bit_array_get(array, 7));
    EXPECT_FALSE(bit_array_get(array, 6));
    EXPECT_TRUE(bit_array_get(array, 8));

    bit_array_free(array);
}


TEST(BitArray, CheckUnset)
{
    BIT_ARRAY* array = bit_array_create(8);
    EXPECT_TRUE(NULL != array);
    EXPECT_EQ(array->num_of_bits, 8);

    for (int i = 0; i < 8; ++i)
    {
        bit_array_set(array, i);
    }

    EXPECT_EQ(*array->buffer, BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7);

    bit_array_unset(array, 0);
    EXPECT_EQ(*array->buffer, 0    + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7);

    bit_array_unset(array, 0);
    EXPECT_EQ(*array->buffer, 0    + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7);

    bit_array_unset(array, 3);
    EXPECT_EQ(*array->buffer, 0    + BIT1 + BIT2 + 0    + BIT4 + BIT5 + BIT6 + BIT7);

    bit_array_unset(array, 7);
    EXPECT_EQ(*array->buffer, 0    + BIT1 + BIT2 + 0    + BIT4 + BIT5 + BIT6 + 0   );

    bit_array_unset(array, 7);
    EXPECT_EQ(*array->buffer, 0    + BIT1 + BIT2 + 0    + BIT4 + BIT5 + BIT6 + 0   );

    bit_array_unset(array, 5);
    EXPECT_EQ(*array->buffer, 0    + BIT1 + BIT2 + 0    + BIT4 + 0    + BIT6 + 0   );

    bit_array_unset(array, 6);
    EXPECT_EQ(*array->buffer, 0    + BIT1 + BIT2 + 0    + BIT4 + 0    + 0    + 0   );

    bit_array_unset(array, 1);
    EXPECT_EQ(*array->buffer, 0    + 0    + BIT2 + 0    + BIT4 + 0    + 0    + 0   );

    bit_array_unset(array, 1);
    EXPECT_EQ(*array->buffer, 0    + 0    + BIT2 + 0    + BIT4 + 0    + 0    + 0   );

    bit_array_unset(array, 2);
    EXPECT_EQ(*array->buffer, 0    + 0    + 0    + 0    + BIT4 + 0    + 0    + 0   );

    bit_array_unset(array, 4);
    EXPECT_EQ(*array->buffer, 0    + 0    + 0    + 0    + 0    + 0    + 0    + 0   );

    bit_array_free(array);
}


TEST(BitArray, CheckEq)
{
    BIT_ARRAY* arrayA = bit_array_create(8);
    EXPECT_TRUE(NULL != arrayA);
    EXPECT_EQ(arrayA->num_of_bits, 8);

    BIT_ARRAY* arrayB = bit_array_create(8);
    EXPECT_TRUE(NULL != arrayB);
    EXPECT_EQ(arrayB->num_of_bits, 8);

    EXPECT_TRUE(bit_array_eq(arrayA, arrayB));

    bit_array_set(arrayA, 4);
    EXPECT_TRUE(!bit_array_eq(arrayA, arrayB));
    bit_array_set(arrayB, 4);
    EXPECT_TRUE(bit_array_eq(arrayA, arrayB));

    bit_array_set(arrayA, 7);
    EXPECT_TRUE(!bit_array_eq(arrayA, arrayB));
    bit_array_set(arrayB, 7);
    EXPECT_TRUE(bit_array_eq(arrayA, arrayB));

    bit_array_set(arrayA, 1);
    EXPECT_TRUE(!bit_array_eq(arrayA, arrayB));
    bit_array_set(arrayB, 1);
    EXPECT_TRUE(bit_array_eq(arrayA, arrayB));

    bit_array_free(arrayA);
    bit_array_free(arrayB);
}


TEST(BitArray, CheckHash)
{
    std::set<uint64_t> all_hashes;

    for (int size = 0; size < 1024; size += 8)
    {
        BIT_ARRAY* array = bit_array_create(size);
        EXPECT_TRUE(NULL != array);
        EXPECT_EQ(array->num_of_bits, size);

        for (int i = 0; i < size; ++i)
        {
            bit_array_set(array, i);
            uint64_t hash = bit_array_hash(array, 0);
            printf("size = %d, i = %d, hash = %lu\n", size, i, hash);
            EXPECT_TRUE(all_hashes.end() == all_hashes.find(hash));
            all_hashes.insert(hash);
        }

        bit_array_free(array);
    }
}


TEST(BitArray, CheckBitwiseOr)
{
    BIT_ARRAY* arrayA = bit_array_create(16);
    EXPECT_TRUE(NULL != arrayA);
    EXPECT_EQ(arrayA->num_of_bits, 16);

    BIT_ARRAY* arrayB = bit_array_create(16);
    EXPECT_TRUE(NULL != arrayB);
    EXPECT_EQ(arrayB->num_of_bits, 16);

    bit_array_set(arrayA, 0);
    bit_array_set(arrayA, 1);
    bit_array_set(arrayA, 4);
    bit_array_set(arrayA, 12);
    bit_array_set(arrayA, 13);

    bit_array_set(arrayB, 0);
    bit_array_set(arrayB, 2);
    bit_array_set(arrayB, 4);
    bit_array_set(arrayB, 10);
    bit_array_set(arrayB, 15);

    bit_array_or(arrayA, arrayA, arrayB);

//  1100 1000 | 1010 1000 = 1110 1000
    EXPECT_EQ(*(arrayA->buffer + 0), 232);
//  0000 1100 | 0010 0001 = 0010 1101
    EXPECT_EQ(*(arrayA->buffer + 1), 45);

    bit_array_free(arrayA);
    bit_array_free(arrayB);
}
