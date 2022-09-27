#ifndef __PAIR_H__
#define __PAIR_H__

// Mess of the C and C++ headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <list>
#include <initializer_list>

struct S_pair {
    char *str_1;
    char *str_2;
};

S_pair *pair_ctor(char *str_1, char *str_2);
int compare(const S_pair &pair_1, const S_pair &pair_2);
void pair_dtor(S_pair *data);
void print_quad(const S_pair &pair_1, const S_pair &pair_2);

//---------- hash function adaptation -----------------
// As I see hash_pair_function just an interface to call hash_2sts_function.
// Hide declaration of the hash_2sts_function from user to .cpp file.
unsigned hash_2sts_function(char *str_1, char *str_2);
unsigned hash_pair_function(const S_pair &strs_pair);

// Try to use std::hash specialization
struct hash_base
{
    std::size_t operator()(const S_pair &s_pair) const
    {
        return hash_pair_function(s_pair);
    }
};
//-------------------------------------------------------

#endif //__PAIR_H__
