#include "strs_pair.hpp"

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
typedef std::unordered_map<S_pair, std::list<S_pair>, my_hash> htab;
#else
typedef std::unordered_map<S_pair, int, my_hash> htab;
#endif


void hashtable_fill(htab &hashtable, struct buffer_t *buffer) {
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
                        auto res = hashtable.emplace(*new_data, std::list<S_pair>{std::initializer_list<S_pair>{*new_data}});
                        #else
                        auto res = hashtable.emplace(*new_data, 1);
                        #endif

                        if(!res.second) {
                            #ifdef PRINT_QUADS
                            res.first->second.push_front(*new_data);
                            #else
                            auto elem = hashtable.find(*new_data);
                            ++elem->second;
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
    for(auto& bkt: hashtable) {

        #ifdef PRINT_QUADS
        size_t sz = bkt.second.size();
        #else
        size_t sz = bkt.second;
        #endif

        if(sz > 1) {
            num_of_quads += sz * (sz - 1) / 2;

            #ifdef PRINT_QUADS
            auto cur = bkt.second.begin();
            while(cur != bkt.second.end()) {
                auto comp = std::next(cur);
                while(comp != bkt.second.end()) {
                    print_quad(*cur, *comp);
                    comp = std::next(comp);
                }
                cur = std::next(cur);
            }
            #endif
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
    htab hashtable;

    hashtable_fill(hashtable, buf);

    quads_num = quads_count(hashtable);
    printf("Num of quads: %u\n", quads_num);
    buffer_dtor(buf);
    
    return 0;
}