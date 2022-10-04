#include <gtest/gtest.h>
#include "hashtable.hpp"

using namespace hashtable;

struct S {
    const char* str;
};

bool operator==(const S &lhs, const S &rhs);
bool operator!=(const S &lhs, const S &rhs);

template<>
struct std::hash<S>
{
    std::size_t operator()(const S &s) const
    {
        const char *tmp = s.str;
        unsigned hash = 5381;
        int c = 0;

        while(c = *tmp++)
            hash = ((hash << 5) + hash) + c;
        
        return hash;
    }
};

TEST(test_find, normal_find) {

    Hashtable<int, int> hashtable{10, 0.75};

    Element elem_1(std::make_pair(1, 1));
    EXPECT_TRUE(hashtable.insert(elem_1));

    Element elem_2(std::make_pair(2, 2));
    EXPECT_TRUE(hashtable.insert(elem_2));

    Element elem_3(std::make_pair(3, 3));
    EXPECT_TRUE(hashtable.insert(elem_3));

    auto res = hashtable.find(1);
    EXPECT_TRUE(res != hashtable.end());
    res = hashtable.find(2);
    EXPECT_TRUE(res != hashtable.end());
    res = hashtable.find(5);
    EXPECT_TRUE(res == hashtable.end());
}

TEST(test_find, big_normal_find) {

    Hashtable<int, int> hashtable{1000000, 0.75};

    for(int i = 0; i < 1000000; ++i) {
        Element elem(std::make_pair(i, i));
        EXPECT_TRUE(hashtable.insert(elem));
    }

    auto res = hashtable.find(986556);
    EXPECT_TRUE(res != hashtable.end());
    res = hashtable.find(1000001);
    EXPECT_TRUE(res == hashtable.end());
}

TEST(test_find, with_solved_collisions) {

    Hashtable<S, int> hashtable{10, 0.75};

    S s1{"foo"}; //hash(foo, 0) = hash(oof, 0)
    S s2{"oof"}; //hash(oof, 1) = hash(bar, 0)
    S s3{"bar"};
    S s4{"rab"};

    Element elem_1(std::make_pair(s1, 1));
    EXPECT_TRUE(hashtable.insert(elem_1));

    Element elem_2(std::make_pair(s2, 2));
    EXPECT_TRUE(hashtable.insert(elem_2));

    Element elem_3(std::make_pair(s3, 3));
    EXPECT_TRUE(hashtable.insert(elem_3));

    auto res = hashtable.find(s1);
    EXPECT_TRUE(res != hashtable.end());
    res = hashtable.find(s2);
    EXPECT_TRUE(res != hashtable.end());
    res = hashtable.find(s4);
    EXPECT_TRUE(res == hashtable.end());
}