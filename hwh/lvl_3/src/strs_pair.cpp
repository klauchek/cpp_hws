#include "strs_pair.hpp"

//---------- hash function adaptation ------------

unsigned hash_2sts_function(char *str_1, char *str_2) {

    unsigned hash = 5381;
    int c = 0;

    while(c = *str_1++)
        hash = ((hash << 5) + hash) + c;

    while(c = *str_2++)
        hash = ((hash << 5) + hash) + c;

    return hash;
}

unsigned hash_pair_function(const S_pair &strs_pair) {

    return hash_2sts_function(strs_pair.str_1, strs_pair.str_2);
}

//-------------------------------------------------------

struct S_pair *pair_ctor(char *str1, char *str2) {
    assert(str1);
    assert(str2);
    
    struct S_pair *new_pair = (struct S_pair *)calloc(1, sizeof(struct S_pair));
    assert(new_pair);
    new_pair->str_1 = str1;
    new_pair->str_2 = str2;
    
    return new_pair;
}

void pair_dtor(S_pair *data) {
    free(data);
}

int compare(const S_pair &pair_1, const S_pair &pair_2) {

    int flag_1 = 0;
    int flag_2 = 0;

    if(strlen(pair_1.str_1) + strlen(pair_1.str_2) != strlen(pair_2.str_1) + strlen(pair_2.str_2))
        return 0;

    char *ptr_1 = pair_1.str_1;
    char *ptr_2 = pair_2.str_1;

    while (*ptr_1) {

        if(*ptr_1 != *ptr_2)
            return 0;
        
        *ptr_1++;

        if(!*ptr_1 && !flag_1) {
            ptr_1 = pair_1.str_2;
            flag_1 = 1;
        }

        *ptr_2++;

        if(!*ptr_2 && !flag_2) {
            ptr_2 = pair_2.str_2;
            flag_2 = 1;
        }
    }

    return 1;
}

void print_quad(S_pair &pair_1, S_pair &pair_2) {
    
    printf("%s %s %s %s\n", pair_1.str_1, pair_1.str_2, pair_2.str_1, pair_2.str_2);
}