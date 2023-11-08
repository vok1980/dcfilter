#include "gtest/gtest.h"
#include "gtest/gtest-param-test.h"

#include "../src/bit_array/bit_array.h"


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
