# My flat map implementation with open addressing
No lists and no elements are stored outside the table, unlike in chaining. The technique used to compute the probe sequences: ***double hashing***.

`hash function:` *h(k, i) = (h1(k) + ih2(k))mod m*,

where *i* - probe number, *h2(k)* - designed so that it always produces an odd number, *m* - number of slots, that is power of 2.
These conditions are a convenient way to ensure that the value *h2(k)* is relatively prime to the hash table size *m* for the entire
hash table to be searched.

# C++ standard & C++ my flat map implementation comparison)
## Compare the duration of execution
to run task cd to `quads` or `dictionary`

build with:
```
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release
cd build
make
```
run task with any test:
```
./hwh_lvl_3 < ../tests/test_1.txt
```
run task with all end2end tests:
```
for f in ../tests/test_*.txt; do ./hwh_lvl_3 < $f; printf '\n'; done
```
# Unit tests (using google test)
if you want to run with unit tests for flat map:
```
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release -DWITH_TESTS
cd build
make
ctest
```
or to see full log:
```
ctest --verbose
```

----------------------------
## lvl_1-2: [dictionary](https://github.com/klauchek/cpp_hws/tree/main/hwh/lvl_4/dictionary) - not interesting to compare
---------------------------

## lvl_3: [quads](https://github.com/klauchek/cpp_hws/tree/main/hwh/lvl_4/quads)

| Test      | my flat map  | std::unordered_map  |
| :--------:|:------------:|:-------------------:|
| test_1    | 0m 0,003s    | 0m 0,003s           |
| test_2    | 0m 0,010s    | 0m 0,003s           |
| test_3    | 0m 0,013s    | 0m 0,004s           |
| test_4    | 0m 0,134s    | 0m 0,110s           |
| test_5    | 0m 3,427s    | 0m 3,623s           |
| test_6    | 0m 13,821s   | 0m 15,841s          |
| test_7    | 0m 30,293s   | 0m 35,951s          |
| test_8    | 0m 55,621s   | 1m 07,406s          |
| test_9    | 1m 00,931s   | 1m 13,639s          |
| test_10   | 1m 16,032s   | 1m 32,470s          |

## *Starting from the test_7 (~6,5K words), flat map wins by 15-17%*
