#include <gtest/gtest.h>
#include "hashtable.hpp"
#include <iostream>

using namespace hashtable;

TEST(test_resize, one_resize) {

    Hashtable<int, int> hashtable{2, 0.75};

    Element elem_1(std::make_pair(1, 1));
    EXPECT_TRUE(hashtable.insert(elem_1));

    Element elem_2(std::make_pair(2, 2));
    EXPECT_TRUE(hashtable.insert(elem_2));

    Element elem_3(std::make_pair(3, 3));
    EXPECT_TRUE(hashtable.insert(elem_3));

    size_t counter = 0;
    for(auto it = hashtable.cbegin(); it != hashtable.cend(); ++it)
        ++counter;
    
    EXPECT_EQ(counter, 4);
    EXPECT_EQ(hashtable.size(), 3);
}

TEST(test_resize, many_resizes) {

    Hashtable<int, int> hashtable{10, 0.75};

    for(int i = 0; i < 1000000; ++i) {
        Element elem(std::make_pair(i, i));
        EXPECT_TRUE(hashtable.insert(elem));
    }

    size_t counter = 0;
    for(auto it = hashtable.cbegin(); it != hashtable.cend(); ++it)
        ++counter;

    EXPECT_EQ(counter, 2097152);
    EXPECT_EQ(1, std::popcount(counter));
    EXPECT_EQ(hashtable.size(), 1000000);
}