#include <gtest/gtest.h>
#include "UniquePtr.hpp"
#include <type_traits>
#include <utility>

struct UniqueTestObject {
    int value = 0;

    void set_value(int new_value) {
        value = new_value;
    }
};

TEST(UniquePtr, default_constructor) {
    UniquePtr<int> pointer;

    EXPECT_EQ(pointer.get(), nullptr);
    EXPECT_FALSE(pointer);
}

TEST(UniquePtr, constructor) {
    UniquePtr<int> pointer(new int(42));

    EXPECT_TRUE(pointer);
    EXPECT_NE(pointer.get(), nullptr);
    EXPECT_EQ(*pointer, 42);
}

TEST(UniquePtr, dereference) {
    UniquePtr<int> pointer(new int(10));

    *pointer = 100;

    EXPECT_EQ(*pointer, 100);
}

TEST(UniquePtr, arrow_operator) {
    UniquePtr<UniqueTestObject> pointer(
        new UniqueTestObject{10}
    );

    EXPECT_EQ(pointer->value, 10);

    pointer->set_value(50);

    EXPECT_EQ(pointer->value, 50);
}

TEST(UniquePtr, move_constructor) {
    UniquePtr<int> first(new int(42));

    int* old_address = first.get();

    UniquePtr<int> second(std::move(first));

    EXPECT_FALSE(first);
    EXPECT_EQ(first.get(), nullptr);

    EXPECT_TRUE(second);
    EXPECT_EQ(second.get(), old_address);
    EXPECT_EQ(*second, 42);
}

TEST(UniquePtr, move_assignment) {
    UniquePtr<int> first(new int(42));
    UniquePtr<int> second(new int(100));

    int* old_address = first.get();

    second = std::move(first);

    EXPECT_FALSE(first);
    EXPECT_EQ(first.get(), nullptr);

    EXPECT_TRUE(second);
    EXPECT_EQ(second.get(), old_address);
    EXPECT_EQ(*second, 42);
}

TEST(UniquePtr, release) {
    UniquePtr<int> pointer(new int(42));

    int* raw_pointer = pointer.release();

    EXPECT_FALSE(pointer);
    EXPECT_EQ(pointer.get(), nullptr);

    ASSERT_NE(raw_pointer, nullptr);
    EXPECT_EQ(*raw_pointer, 42);

    delete raw_pointer;
}

TEST(UniquePtr, reset) {
    UniquePtr<int> pointer(new int(42));

    pointer.reset();

    EXPECT_FALSE(pointer);
    EXPECT_EQ(pointer.get(), nullptr);
}

TEST(UniquePtr, reset_pointer) {
    UniquePtr<int> pointer(new int(10));

    pointer.reset(new int(20));

    EXPECT_TRUE(pointer);
    EXPECT_NE(pointer.get(), nullptr);
    EXPECT_EQ(*pointer, 20);
}

TEST(UniquePtr, reset_same_pointer) {
    UniquePtr<int> pointer(new int(42));

    int* old_address = pointer.get();

    pointer.reset(old_address);

    EXPECT_TRUE(pointer);
    EXPECT_EQ(pointer.get(), old_address);
    EXPECT_EQ(*pointer, 42);
}

TEST(UniquePtr, array) {
    UniquePtr<int[]> array(
        new int[3]{10, 20, 30}
    );

    EXPECT_TRUE(array);
    EXPECT_NE(array.get(), nullptr);

    EXPECT_EQ(array[0], 10);
    EXPECT_EQ(array[1], 20);
    EXPECT_EQ(array[2], 30);
}

TEST(UniquePtr, array_change) {
    UniquePtr<int[]> array(
        new int[3]{10, 20, 30}
    );

    array[0] = 100;
    array[1] = 200;
    array[2] = 300;

    EXPECT_EQ(array[0], 100);
    EXPECT_EQ(array[1], 200);
    EXPECT_EQ(array[2], 300);
}

TEST(UniquePtr, array_release) {
    UniquePtr<int[]> array(
        new int[3]{10, 20, 30}
    );

    int* raw_array = array.release();

    EXPECT_FALSE(array);
    EXPECT_EQ(array.get(), nullptr);

    ASSERT_NE(raw_array, nullptr);
    EXPECT_EQ(raw_array[0], 10);
    EXPECT_EQ(raw_array[1], 20);
    EXPECT_EQ(raw_array[2], 30);

    delete[] raw_array;
}