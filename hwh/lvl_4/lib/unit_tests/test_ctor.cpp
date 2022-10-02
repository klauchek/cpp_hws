#include <gtest/gtest.h>
#include "hashtable.hpp"

using namespace hashtable;
using htab = Hashtable<int, int>;

TEST(test_ctor, normal_ctor) {
    htab hashtable{4, 0.75};

    size_t counter = 0;
    for(auto it = hashtable.cbegin(); it != hashtable.cend(); ++it)
        ++counter;

    EXPECT_EQ(counter, 4);
    EXPECT_EQ(hashtable.size(), 0);
    EXPECT_EQ(hashtable.threshold(), 0.75);

}

TEST(test_ctor, invariants_violation_1) {
    htab hashtable{10, 0.75};

    size_t counter = 0;
    for(auto it = hashtable.cbegin(); it != hashtable.cend(); ++it)
        ++counter;

    EXPECT_NE(counter, 10);
    EXPECT_EQ(1, std::popcount(counter));
    EXPECT_EQ(hashtable.size(), 0);
    EXPECT_EQ(hashtable.threshold(), 0.75);
}


TEST(test_ctor, invariants_violation_2) {
    htab hashtable{4, 2};

    size_t counter = 0;
    for(auto it = hashtable.cbegin(); it != hashtable.cend(); ++it)
        ++counter;
    EXPECT_EQ(counter, 4);
    EXPECT_EQ(hashtable.size(), 0);
    EXPECT_NE(hashtable.threshold(), 2);
}
