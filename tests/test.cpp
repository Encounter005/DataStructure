#include <gtest/gtest.h>

#include "../src/MyArray.hpp"
#include <gtest/gtest.h>
#include <initializer_list>
#include <algorithm>

// 测试默认构造函数
TEST(MyArrayTest, DefaultConstructor) {
    MyArray<int, 10> arr;
    EXPECT_EQ(arr.size(), 10);  // 假设 size() 返回数组大小
}


TEST(MyArrayTest, CopyConstructor) {
    MyArray<int, 3> arr{1 , 2 , 3};
    MyArray<int, 3> b(arr);
    EXPECT_EQ(b.size(), 3);  // 假设 size() 返回数组大小
}
TEST(MyArrryTest, MoveConstructor) {
    MyArray<int, 3> arr{1 , 2 , 3};
    MyArray<int, 3> b(std::move(arr));
    EXPECT_EQ(b.size(), 3);  // 假设 size() 返回数组大小
}

// 测试初始化列表构造函数
TEST(MyArrayTest, InitializerListConstructor) {
    std::initializer_list<int> ilist{1, 2, 3, 4, 5};
    MyArray<int, 5> arr(ilist);

    int expected[] = {1, 2, 3, 4, 5};
    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

// 测试插入功能
TEST(MyArrayTest, Insert) {
    MyArray<int, 10> arr{1, 2, 3, 4, 5};
    arr.insert(101);

    int expected[] = {1, 2, 3, 4, 5, 101};
    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

// 测试删除功能
TEST(MyArrayTest, Remove) {
    MyArray<int, 10> arr{1, 2, 3, 4, 5};
    arr.remove(2);  // 假设 remove 删除指定索引的元素

    int expected[] = {1, 2, 4, 5};
    for (size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

// 测试交换功能
TEST(MyArrayTest, Swap) {
    MyArray<int, 10> a{1, 2, 3};
    MyArray<int, 10> b{4, 5, 6};

    std::swap(a, b);

    int expectedA[] = {4, 5, 6};
    int expectedB[] = {1, 2, 3};

    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(a[i], expectedA[i]);
        EXPECT_EQ(b[i], expectedB[i]);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

