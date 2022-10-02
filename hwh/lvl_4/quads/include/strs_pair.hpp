#ifndef __PAIR_H__
#define __PAIR_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <functional>
#include <list>
#include <initializer_list>

struct S_pair {
    char *str_1;
    char *str_2;
};

S_pair *pair_ctor(char *str_1, char *str_2);               //no need for this vers of hashtable
int compare(const S_pair &pair_1, const S_pair &pair_2);
void pair_dtor(S_pair *data);                              //no need for this vers of hashtable
void print_quad(const S_pair &pair_1, const S_pair &pair_2);

//---------- hash function adaptation -----------------
unsigned hash_pair_function(const S_pair &strs_pair);

template<>
struct std::hash<S_pair>
{
    std::size_t operator()(const S_pair &s_pair) const
    {
        return hash_pair_function(s_pair);
    }
};

//-------------------------------------------------------

#endif //__PAIR_H__