#include <gtest/gtest.h>
#include "hashtable.hpp"

using namespace hashtable;

struct S {
    const char *str;
};

bool operator==(const S &lhs, const S &rhs) {
    if(!(strcmp(lhs.str, rhs.str)))
        return 1;
    else return 0;
}

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

TEST(test_insert, normal_insert) {

    Hashtable<int, int> hashtable{10, 0.75};

    Element elem_1(std::make_pair(1, 1));
    EXPECT_TRUE(hashtable.insert(elem_1));

    Element elem_2(std::make_pair(2, 2));
    EXPECT_TRUE(hashtable.insert(elem_2));

    Element elem_3(std::make_pair(3, 3));
    EXPECT_TRUE(hashtable.insert(elem_3));

    EXPECT_EQ(hashtable.size(), 3);
}

TEST(test_insert, big_normal_insert) {

    Hashtable<int, int> hashtable{1000000, 0.75};

    for(int i = 0; i < 1000000; ++i) {
        Element elem(std::make_pair(i, i));
        EXPECT_TRUE(hashtable.insert(elem));
    }

    EXPECT_EQ(hashtable.size(), 1000000);
}

TEST(test_insert, with_equal_elems) {
    
    Hashtable<S, int> hashtable{10, 0.75};

    S s1{"abc"};
    S s2{"abc"};
    S s3{"def"};

    Element elem_1(std::make_pair(s1, 1));
    EXPECT_TRUE(hashtable.insert(elem_1));

    Element elem_2(std::make_pair(s2, 2));
    EXPECT_FALSE(hashtable.insert(elem_2));

    Element elem_3(std::make_pair(s3, 3));
    EXPECT_TRUE(hashtable.insert(elem_3));

    EXPECT_EQ(hashtable.size(), 2);
}

TEST(test_insert, with_many_equal_elems) {

    Hashtable<S, int> hashtable{1000000, 0.75};

    for(int i = 0; i < 1000000; ++i) {
        S s{"abc"};
        Element elem(std::make_pair(s, i));
        if(i == 0)
            EXPECT_TRUE(hashtable.insert(elem));
        else
            EXPECT_FALSE(hashtable.insert(elem));
    }

    EXPECT_EQ(hashtable.size(), 1);
}


TEST(test_insert, with_solved_collisions) {

    Hashtable<S, int> hashtable{10, 0.75};

    S s1{"foo"}; //hash(foo, 0) = hash(oof, 0)
    S s2{"oof"}; //hash(oof, 1) = hash(bar, 0)
    S s3{"bar"};

    Element elem_1(std::make_pair(s1, 1));
    EXPECT_TRUE(hashtable.insert(elem_1));

    Element elem_2(std::make_pair(s2, 2));
    EXPECT_TRUE(hashtable.insert(elem_2));

    Element elem_3(std::make_pair(s3, 3));
    EXPECT_TRUE(hashtable.insert(elem_3));

    EXPECT_EQ(hashtable.size(), 3);
    
}