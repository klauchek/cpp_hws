# My flat map implementation with open addressing
No lists and no elements are stored outside the table, unlike in chaining. The technique used to compute the probe sequences: ***double hashing***.

`hash function:` *h(k, i) = (h1(k) + ih2(k))mod m*,

where *i* - probe number, *h2(k)* - designed so that it always produces an odd number, *m* - number of slots, that is power of 2.
These conditions are a convenient way to ensure that the value *h2(k)* is relatively prime to the hash table size *m* for the entire
hash table to be searched.

# C++ standard & C++ my flat map implementation comparison)
## Compare the duration of execution
----------------------------
## lvl_1-2: [dictionary](https://github.com/klauchek/cpp_hws/tree/main/hwh/lvl_4/dictionary) - not interesting to compare
---------------------------

## lvl_3: [quads](https://github.com/klauchek/cpp_hws/tree/main/hwh/lvl_4/quads)

| Test      | my flat map  | std::unordered_map  |
| :--------:|:------------:|:-------------------:|
| test_1    | 0m 0,003s    | 0m 0,003s           |
| test_2    | 0m 0,010s    | 0m 0,003s           |
| test_3    | 0m 0,014s    | 0m 0,005s           |
| test_4    | 0m 0,151s    | 0m 0,122s           |
| test_5    | 0m 3,488s    | 0m 3,771s           |
| test_6    | 0m 13,893s   | 0m 16,463s          |
| test_7    | 0m 30,474s   | 0m 37,202s          |
| test_8    | 0m 55,703s   | 1m 08,852s          |
| test_9    | 1m 00,786s   | 1m 15,541s          |
| test_10   | 1m 14,973s   | 1m 34,905s          |

## *Starting from the test_7 (~6,5K words), flat map wins by 18-21%*
