#include <gtest/gtest.h>
#include "SharedPtr.hpp"
#include <utility>

struct TestObject {
    int value = 0;

    void set_value(int new_value) {
        value = new_value;
    }
};

TEST(SharedPtr, default_constructor) {
    SharedPtr<int> pointer;

    EXPECT_EQ(pointer.get(), nullptr);
    EXPECT_EQ(pointer.use_count(), 0);
    EXPECT_FALSE(pointer);
}

TEST(SharedPtr, constructor) {
    SharedPtr<int> pointer(new int(42));

    EXPECT_TRUE(pointer);
    EXPECT_NE(pointer.get(), nullptr);
    EXPECT_EQ(pointer.use_count(), 1);
    EXPECT_EQ(*pointer, 42);
}

TEST(SharedPtr, dereference) {
    SharedPtr<int> pointer(new int(10));

    *pointer = 100;

    EXPECT_EQ(*pointer, 100);
}

TEST(SharedPtr, arrow_operator) {
    SharedPtr<TestObject> pointer(
        new TestObject{10}
    );

    EXPECT_EQ(pointer->value, 10);

    pointer->set_value(50);

    EXPECT_EQ(pointer->value, 50);
}

TEST(SharedPtr, copy_constructor) {
    SharedPtr<int> first(new int(42));

    {
        SharedPtr<int> second(first);

        EXPECT_EQ(first.use_count(), 2);
        EXPECT_EQ(second.use_count(), 2);
        EXPECT_EQ(first.get(), second.get());
        EXPECT_EQ(*second, 42);
    }

    EXPECT_EQ(first.use_count(), 1);
}

TEST(SharedPtr, copy_assignment) {
    SharedPtr<int> first(new int(10));
    SharedPtr<int> second(new int(20));

    second = first;

    EXPECT_EQ(first.use_count(), 2);
    EXPECT_EQ(second.use_count(), 2);
    EXPECT_EQ(first.get(), second.get());
    EXPECT_EQ(*second, 10);
}

TEST(SharedPtr, self_assignment) {
    SharedPtr<int> pointer(new int(42));

    pointer = pointer;

    EXPECT_TRUE(pointer);
    EXPECT_EQ(pointer.use_count(), 1);
    EXPECT_EQ(*pointer, 42);
}

TEST(SharedPtr, move_constructor) {
    SharedPtr<int> first(new int(42));

    int* address = first.get();

    SharedPtr<int> second(std::move(first));

    EXPECT_FALSE(first);
    EXPECT_EQ(first.get(), nullptr);
    EXPECT_EQ(first.use_count(), 0);

    EXPECT_TRUE(second);
    EXPECT_EQ(second.get(), address);
    EXPECT_EQ(second.use_count(), 1);
    EXPECT_EQ(*second, 42);
}

TEST(SharedPtr, move_assignment) {
    SharedPtr<int> first(new int(42));
    SharedPtr<int> second(new int(100));

    int* address = first.get();

    second = std::move(first);

    EXPECT_FALSE(first);
    EXPECT_EQ(first.get(), nullptr);
    EXPECT_EQ(first.use_count(), 0);

    EXPECT_TRUE(second);
    EXPECT_EQ(second.get(), address);
    EXPECT_EQ(second.use_count(), 1);
    EXPECT_EQ(*second, 42);
}

TEST(SharedPtr, reset) {
    SharedPtr<int> pointer(new int(42));

    pointer.reset();

    EXPECT_FALSE(pointer);
    EXPECT_EQ(pointer.get(), nullptr);
    EXPECT_EQ(pointer.use_count(), 0);
}

TEST(SharedPtr, reset_pointer) {
    SharedPtr<int> pointer(new int(10));

    pointer.reset(new int(20));

    EXPECT_TRUE(pointer);
    EXPECT_EQ(pointer.use_count(), 1);
    EXPECT_EQ(*pointer, 20);
}

TEST(SharedPtr, array) {
    SharedPtr<int[]> array(
        new int[3]{10, 20, 30}
    );

    EXPECT_TRUE(array);
    EXPECT_EQ(array.use_count(), 1);

    EXPECT_EQ(array[0], 10);
    EXPECT_EQ(array[1], 20);
    EXPECT_EQ(array[2], 30);

    array[1] = 200;

    EXPECT_EQ(array[1], 200);
}

TEST(SharedPtr, array_copy) {
    SharedPtr<int[]> first(
        new int[3]{10, 20, 30}
    );

    {
        SharedPtr<int[]> second = first;

        EXPECT_EQ(first.use_count(), 2);
        EXPECT_EQ(second.use_count(), 2);
        EXPECT_EQ(first.get(), second.get());

        second[0] = 100;

        EXPECT_EQ(first[0], 100);
    }

    EXPECT_EQ(first.use_count(), 1);
}
