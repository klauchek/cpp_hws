#include <gtest/gtest.h>
#include "hashtable.hpp"
#include <iostream>

using namespace hashtable;

TEST(test_erase, normal_erase) {

    Hashtable<int, int> hashtable{10, 0.75};

    Element elem_1(std::make_pair(1, 1));
    EXPECT_TRUE(hashtable.insert(elem_1));

    Element elem_2(std::make_pair(2, 2));
    EXPECT_TRUE(hashtable.insert(elem_2));

    auto res = hashtable.find(1);
    EXPECT_TRUE(res != hashtable.end());

    EXPECT_TRUE(hashtable.erase(1));

    res = hashtable.find(1);
    EXPECT_TRUE(res == hashtable.end());

    EXPECT_FALSE(hashtable.erase(1));
}

TEST(test_erase, big_normal_erase) {

    Hashtable<int, int> hashtable{1000000, 0.75};

    for(int i = 0; i < 1000000; ++i) {
        Element elem(std::make_pair(i, i));
        EXPECT_TRUE(hashtable.insert(elem));
    }

    for(int i = 0; i < 1000000; ++i)
        EXPECT_TRUE(hashtable.erase(i));
}

TEST(test_erase, insert_after_erase) {

    Hashtable<int, int> hashtable{2, 0.75};

    Element elem_1(std::make_pair(1, 1));
    EXPECT_TRUE(hashtable.insert(elem_1));

    Element elem_2(std::make_pair(2, 2));
    EXPECT_TRUE(hashtable.insert(elem_2));

    EXPECT_TRUE(hashtable.erase(1));

    Element elem_3(std::make_pair(1, 1));
    EXPECT_TRUE(hashtable.insert(elem_3));

    size_t counter = 0;
    for(auto it = hashtable.cbegin(); it != hashtable.cend(); ++it)
        ++counter;

    EXPECT_EQ(counter, 2);
    EXPECT_EQ(hashtable.size(), 2);
}

TEST(test_erase, insert_after_all_erased) {

    Hashtable<int, int> hashtable{128, 0.75};

    for(int i = 0; i < 100; ++i) {
        Element elem(std::make_pair(i, i));
        EXPECT_TRUE(hashtable.insert(elem));
    }

    for(int i = 0; i < 100; ++i)
        EXPECT_TRUE(hashtable.erase(i));

    EXPECT_EQ(hashtable.size(), 0);
    size_t counter = 0;
    for(auto it = hashtable.cbegin(); it != hashtable.cend(); ++it)
        ++counter;

    EXPECT_EQ(counter, 256);

    //inserting after deletion
    for(int i = 0; i < 128; ++i) {
        Element elem(std::make_pair(i, i));
        EXPECT_TRUE(hashtable.insert(elem));
    }
    EXPECT_EQ(hashtable.size(), 128);
    counter = 0;
    for(auto it = hashtable.cbegin(); it != hashtable.cend(); ++it)
        ++counter;

    EXPECT_EQ(counter, 256);
}