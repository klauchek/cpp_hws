#include "strs_pair.hpp"
#include "hashtable.hpp"
#include <iostream>

#define START_BUF_CAPACITY 128

bool operator==(const S_pair &lhs, const S_pair &rhs) {
    return compare(lhs, rhs);
}

//------------------------- READING INPUT ----------------------------//
struct buffer_t {
    char *strs_arr;
    unsigned size;
    unsigned capacity; 
};

void buffer_resize(struct buffer_t *buffer) {
    assert(buffer);

    buffer->capacity *= 2;
    buffer->strs_arr = (char *)realloc(buffer->strs_arr, buffer->capacity * sizeof(char));
    for(int i = buffer->size; i < buffer->capacity; ++i)
        buffer->strs_arr[i] = 0;
}

struct buffer_t *make_buffer(struct buffer_t *buffer, unsigned num_of_strs) {

    buffer = (struct buffer_t *)calloc(1, sizeof(struct buffer_t));
    assert(buffer);
    buffer->capacity = START_BUF_CAPACITY;

    buffer->strs_arr = (char *)calloc(buffer->capacity, sizeof(char));
    assert(buffer->strs_arr);

    unsigned counter = 0; 

    char sym = getchar();
    
    while((sym = getchar()) != EOF) {
    
        if(buffer->size == buffer->capacity / 2)
            buffer_resize(buffer);
        if (sym == '\n')
            buffer->strs_arr[buffer->size] = '\0';
        else    
            buffer->strs_arr[buffer->size] = sym;
        ++buffer->size;
    }
    ++counter;
    
    return buffer;
}

void buffer_dtor(struct buffer_t *buffer) {
    assert(buffer);

    free(buffer->strs_arr);
    free(buffer);
}

//------------------------- COUNTING QUADS ----------------------------//

#ifdef PRINT_QUADS
typedef hashtable::Hashtable<S_pair, std::list<S_pair>, hash_base> htab;
#else
typedef hashtable::Hashtable<S_pair, int, hash_base> htab;
#endif


void hashtable_fill(htab &hashtable, struct buffer_t *buffer) {
    assert(buffer);
    
    unsigned key = 0;
    char *word1 = NULL;
    char *word2 = NULL;
    
    unsigned buf_len = buffer->size;
    char *text_buffer = buffer->strs_arr;

    for (int i = 0; i < buf_len; ++i) {
        if(isalpha(text_buffer[i])) {
            for (int j = 0; j < buf_len; ++j) {
                if(isalpha(text_buffer[j])) {
                    if(i != j) {
                        struct S_pair *new_data = pair_ctor(text_buffer + i, text_buffer + j);

                        #ifdef PRINT_QUADS
                        bool res = hashtable.insert(std::make_pair(*new_data, std::list<S_pair>{std::initializer_list<S_pair>{*new_data}}));
                        #else
                        bool res = hashtable.insert(std::make_pair(*new_data, 1));
                        #endif

                        if(!res) {
                            auto elem = hashtable.find(*new_data);
                            #ifdef PRINT_QUADS
                            elem->data.push_front(*new_data);
                            #else
                            ++elem->data;
                            #endif
                        }
                        pair_dtor(new_data);
                    }
                    j += strlen(text_buffer + j);
                }
            }
            i += strlen(text_buffer + i);
        }
    }
}

unsigned quads_count(htab &hashtable) {

    unsigned num_of_quads = 0;
    for(auto it = hashtable.begin(); it != hashtable.end(); ++it) {
        if(it->type == hashtable::kFull) {
            #ifdef PRINT_QUADS
            size_t sz = it->data.size();
            #else
            size_t sz = it->data;
            #endif

            if(sz > 1) {
                num_of_quads += sz * (sz - 1) / 2;

            #ifdef PRINT_QUADS
            auto cur = it->data.begin();
            while(cur != it->data.end()) {
                auto comp = std::next(cur);
                while(comp != it->data.end()) {
                    print_quad(*cur, *comp);
                    comp = std::next(comp);
                }
                cur = std::next(cur);
            }
            #endif
            }
        }
    }
    return num_of_quads;
}

//------------------------- MAIN ----------------------------//
int main() {
    int res = 0;
    unsigned strs_amount = 0;
    unsigned buf_len = 0;
    unsigned quads_num = 0;

    struct buffer_t *buf = NULL;

    res = scanf("%d", &strs_amount);
    if (res != 1) {
        printf("%s\n", "Wrong input of strings amount");
        abort();
    }

    buf = make_buffer(buf, strs_amount);
    htab hashtable{8, 0.75};

    hashtable_fill(hashtable, buf);

    quads_num = quads_count(hashtable);
    printf("Num of quads: %u\n", quads_num);
    buffer_dtor(buf);
    
    return 0;
}