#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <iostream>
#include <vector>
#include <cassert>
#include <utility>

namespace hashtable {

enum Ctrl {
    kFull,           
    kEmpty,
    kDeleted
};

template <typename KeyT, typename T>
struct Element {
    KeyT key;
    T data;
    Ctrl type = kEmpty;
};

template <typename KeyT, typename T>
using VecIt = typename std::vector<Element<KeyT, T>>::iterator;

template <typename KeyT, typename T, typename HashT>
class Hashtable {
    std::vector<Element<KeyT, T>> elements_;
    size_t size_ = 0;
    size_t capacity_;
    double threshold_;
    HashT hash_base;

    //--------------------HASHING STRATEGY----------------------
    size_t hash1(const KeyT &key) const {
        return hash_base(key) % capacity_;
    }
    size_t hash2(const KeyT &key) const {
        size_t x = (hash_base(key) % (capacity_ - 2));
        if (x % 2 == 0)
            return x + 1;
        return x; 
    }
    size_t hash(const KeyT &key, const size_t probe_num) const {   
        return (hash1(key) + probe_num * hash2(key)) % capacity_;
    }
    //-----------------------------------------------------------

    void resize();

public:

    Hashtable(size_t capacity, double threshold) : capacity_(capacity), threshold_(threshold), elements_(capacity) {}
    bool insert(std::pair<KeyT, T> pair);
    VecIt<KeyT, T> find(const KeyT &key);

    size_t size() const {
        return size_;
    }
    VecIt<KeyT, T> end() {
        return elements_.end();
    }
    VecIt<KeyT, T> begin() {
        return elements_.begin();
    }
};


template <typename KeyT, typename T, typename HashT>
bool Hashtable<KeyT, T, HashT>::insert(std::pair<KeyT, T> pair) {
    Element<KeyT, T> elem{pair.first, pair.second, kFull};
    if ((static_cast<double>(size_) / capacity_) >= threshold_)
        resize();
    size_t probe_num = 0;
    while(probe_num < capacity_) {
        size_t pos = hash(elem.key, probe_num);
        Element<KeyT, T> &vec_elem = elements_[pos];
        if(vec_elem.type == kFull && elem.key == vec_elem.key) {
            return false;
        }
        if(vec_elem.type == kEmpty) {
            vec_elem = elem;
            ++size_;
            return true;
        }
        ++probe_num;
    }
    return false;
}

template <typename KeyT, typename T, typename HashT>
VecIt<KeyT, T> Hashtable<KeyT, T, HashT>::find(const KeyT &key){
    size_t probe_num = 0;
    while(probe_num < capacity_) {
        size_t pos = hash(key, probe_num);
        Element<KeyT, T> &vec_elem = elements_[pos];
        if(vec_elem.type == kFull && key == vec_elem.key)
            return elements_.begin() + pos;
        if(vec_elem.type == kEmpty)
            return elements_.end();
        ++probe_num;
    }
    return elements_.end();
}

template <typename KeyT, typename T, typename HashT>
void Hashtable<KeyT, T, HashT>::resize() {
    std::vector<Element<KeyT, T>> tmp_vec(capacity_ * 2);
    size_t tmp_size = 0;
    capacity_ *= 2;

    for(auto it : elements_) {
        if(it.type == kFull) {
           size_t probe_num = 0;
           while(probe_num < capacity_) {
                size_t pos = hash(it.key, probe_num);
                Element<KeyT, T> &tmp_vec_elem = tmp_vec[pos];
                if(tmp_vec_elem.type == kFull && it.key == tmp_vec_elem.key)
                    break;
                if(tmp_vec_elem.type == kEmpty) {
                    tmp_vec_elem = it;
                    ++tmp_size;
                    break;
                }
                ++probe_num;
           }
        }
    }

    size_ = tmp_size;
    elements_.swap(tmp_vec);
}

} //namespace hashtable



#endif //__HASH_TABLE_H__