#include "hashtable.hpp"
#include <cstring>

#define START_CAPACITY 128
#define HASH_THRESHOLD 0.75

struct S {
    char *str;
};

bool operator==(const S &lhs, const S &rhs) {
    if(!(strcmp(lhs.str, rhs.str)))
        return 1;
    else return 0;
}

std::ostream& operator<<(std::ostream& out, const S& s)
{
   return out << s.str;
}

struct hash_base
{
    std::size_t operator()(const S &s) const
    {
        char *tmp = s.str;
        unsigned hash = 5381;
        int c = 0;

        while(c = *tmp++)
            hash = ((hash << 5) + hash) + c;
        
        return hash;
    }
};


//------------------------- READING INPUT ----------------------------//

char* make_buffer(unsigned buf_len) {

    char *buffer = (char *)calloc(buf_len + 1, sizeof(char));
    assert(buffer);

    char sym = getchar();

    for(int i = 0; i < buf_len; ++i) {

        sym = getchar();
        if (sym == ' ' || sym == '\n')
            buffer[i] = '\0';
        else    
            buffer[i] = sym;
    }
    return buffer;
}
//------------------------- COUNTING FREQUENCY OF WORDS ----------------------------//

void hashtable_fill(hashtable::Hashtable<S, int, hash_base> &hashtable, char *buf, unsigned buf_len) {
    assert(buf);

    for(int i = 0; i < buf_len; ++i)
    {
        
        if(isalpha(buf[i])) {

            struct S str = { buf + i };
            bool res = hashtable.insert(std::make_pair(str, 1));
   
            if(!res) {
                auto elem = hashtable.find(str);
                ++elem->data;
            }
            i += strlen(buf + i);
        }
    }
}

void freq_count(hashtable::Hashtable<S, int, hash_base> &hashtable, unsigned w_buf_len) {

    char *words_buf = NULL;

    words_buf = make_buffer(w_buf_len);
    for(int i = 0; i < w_buf_len; ++i)
    {
        if(isalpha(words_buf[i])) {
            struct S str = { words_buf + i };
            auto find = hashtable.find(str);
            if(find != hashtable.end())
                printf("%d ", find->data);
            else
                printf("0 ");

            i += strlen(words_buf + i);
        }
    }

    free(words_buf);
}

//------------------------- MAIN ----------------------------//

int main () {
    int res = 0;
    unsigned words_amount = 0;
    unsigned buf_len = 0;
    unsigned w_buf_len = 0;
    char* buffer;

    res = scanf("%d %d", &words_amount, &buf_len);
    if (res != 2) {
        printf("%s\n", "Wrong input of words amount and text buffer lenght");
        abort();
    }

    hashtable::Hashtable<S, int, hash_base> hashtable {START_CAPACITY, HASH_THRESHOLD};
    buffer = make_buffer(buf_len);
    hashtable_fill(hashtable, buffer, buf_len);

    res = scanf("%d", &w_buf_len);
    if (res != 1) {
        printf("%s\n", "Wrong input of words buffer lenght");
        abort();
    }

    freq_count(hashtable, w_buf_len);
    free(buffer);
}