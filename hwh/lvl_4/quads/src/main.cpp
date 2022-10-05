#include "strs_pair.hpp"
#include "hashtable.hpp"
#include <iostream>

#define START_BUF_CAPACITY 128

bool operator==(const S_pair &lhs, const S_pair &rhs) {
    return compare(lhs, rhs);
}
bool operator!=(const S_pair &lhs, const S_pair &rhs) {
    // If you decide to rewrite main logic of operator==, you have to rewrite and operator!=.
    // Try to reuse operator==.
    return (!compare(lhs, rhs));
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

struct buffer_t *make_buffer(unsigned num_of_strs) {

    struct buffer_t *buffer = (struct buffer_t *)calloc(1, sizeof(struct buffer_t));
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
typedef hashtable::Hashtable<S_pair, std::list<S_pair>> htab;
#else
typedef hashtable::Hashtable<S_pair, int> htab;
#endif


htab hashtable_create(struct buffer_t *buffer, size_t cap, double threshold) {
    assert(buffer);
    
    htab hashtable{cap, threshold};
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
                        S_pair new_data {text_buffer + i, text_buffer + j};
                        #ifdef PRINT_QUADS
                        hashtable::Element new_elem(std::make_pair(new_data, std::list<S_pair>{new_data}));
                        #else
                        hashtable::Element new_elem(std::make_pair(new_data, 1));
                        #endif
                        bool res = hashtable.insert(new_elem);

                        if(!res) {
                            auto elem = hashtable.find(new_data);
                            #ifdef PRINT_QUADS
                            elem->data.push_front(new_data);
                            #else
                            ++elem->data;
                            #endif
                        }
                    }
                    j += strlen(text_buffer + j);
                }
            }
            i += strlen(text_buffer + i);
        }
    }
    return hashtable;
}

unsigned quads_count(htab const &hashtable) {

    unsigned num_of_quads = 0;
    for(auto it = hashtable.cbegin(); it != hashtable.cend(); ++it) {
        if(it->type == hashtable::kFull) {
            #ifdef PRINT_QUADS
            size_t sz = it->data.size();
            #else
            size_t sz = it->data;
            #endif

            if(sz > 1) {
                num_of_quads += sz * (sz - 1) / 2;

            #ifdef PRINT_QUADS
            for (auto cur = it->data.begin(), end = it->data.end(); cur != end; ++cur)
                for (auto comp = std::next(cur); comp != end; ++comp)
                    print_quad(*cur, *comp);
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
        return 1;
    }

    buf = make_buffer(strs_amount);
    htab hashtable = hashtable_create(buf, 16, 2);

    quads_num = quads_count(hashtable);
    printf("Num of quads: %u\n", quads_num);
    buffer_dtor(buf);
    
    return 0;
}
