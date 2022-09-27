#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

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

// TODO try to name each entity for better readability
// template <typename KeyT, typename T>
// using VecOfElems = std::vector<Element<KeyT, T>>;
template <typename KeyT, typename T>
// using VecIt = typename VecOfElems<KeyT, T>::iterator;
using VecIt = typename std::vector<Element<KeyT, T>>::iterator;

template <typename KeyT, typename T, typename HashT>
class Hashtable {
    // TODO move usings above inside class declaration since they make sence only inside Hashtable.
    // VecOfElems<KeyT, T> elements_;
    std::vector<Element<KeyT, T>> elements_;
    size_t size_ = 0;
    // TODO this invariant has to be checked inside Hashtable.
    // What if I call Hashtable<smth> hash{10, 50.0}; ??
    size_t capacity_;    //atterntion! it should be pow of 2 because of hashing strategy
    double threshold_;
    // May be it is better to use std::hash specialization for KeyT?
    HashT hash_base;

    //--------------------HASHING STRATEGY----------------------
    size_t hash1(const KeyT &key) const {
        return hash_base(key) % capacity_;
    }
    size_t hash2(const KeyT &key) const {
        // TODO with condition capacity = 2^x you can do smth interesting with bit manipulation.
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
    // TODO std::pair<KeyT, T> almost the same as Element.
    // May be it is better to do some form of cast via constructor or operator?
    bool insert(std::pair<KeyT, T> pair);
    VecIt<KeyT, T> find(const KeyT &key);
    bool erase(size_t pos);

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
    // TODO why wont use for (size_t probe_num = 0; probe_num < capacity_; ++probe_num) { ??
    // TODO the loop below occurs in several more places (insert, find, resize). Please try to extract similar logic into some function and reuse it.
    size_t probe_num = 0;
    while(probe_num < capacity_) {
        size_t pos = hash(elem.key, probe_num);
        // TODO are we sure that hash returns proper pos? What if it reaches out of bounds?
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

template <typename KeyT, typename T, typename HashT>
bool Hashtable<KeyT, T, HashT>::erase(size_t pos) {
    if(pos >= capacity_) {
        return false;
    }

    Element<KeyT, T> &elem = elements_[pos];
    if (elem.type == kFull) {
        elem.type = kDeleted;
        return true;
    }
    return false;
}

} //namespace hashtable



#endif //__HASH_TABLE_H__
