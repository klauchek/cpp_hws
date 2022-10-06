#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <vector>
#include <utility>
#include <bit>
#include <cmath>
#include <iostream>

namespace hashtable {

const double DEFAULT_THRESHOLD = 0.75;

enum class Ctrl {
    kFull,           
    kEmpty,
    kDeleted
};

template <typename KeyT, typename T>
struct Element {
    KeyT key{};
    T data{};
    Ctrl type = Ctrl::kEmpty;

    Element() {}
    Element(std::pair<KeyT, T> pair): key(pair.first), data(pair.second), type(Ctrl::kFull) {}
};

template <typename KeyT, typename T>
class Hashtable {

    using VecOfElems = typename std::vector<Element<KeyT, T>>;
    using VecIt = typename VecOfElems::iterator;
    using CVecIt = typename VecOfElems::const_iterator;

    size_t size_ = 0;
    size_t capacity_;
    VecOfElems elements_;
    double threshold_;

    //--------------------HASHING STRATEGY----------------------
    size_t hash1(const KeyT &key) const {
        return std::hash<KeyT>{}(key) & (capacity_ - 1);
    }
    size_t hash2(const KeyT &key) const {
        size_t x = (std::hash<KeyT>{}(key) & ((capacity_ >> 1) - 1));
        return x | 1u;
    }
    size_t hash(const KeyT &key, const size_t probe_num) const {   
        return (hash1(key) + probe_num * hash2(key)) & (capacity_ - 1);
    }
    //-----------------------------------------------------------

    void swap(Hashtable &other);
    void resize();
    void fill_empty(VecIt elem, Element<KeyT, T> &to_insert);
    size_t verify_cap(const size_t capacity) const;
    double verify_thres(const double threshold) const;

public:

    Hashtable(size_t capacity, double threshold) : capacity_{verify_cap(capacity)}, threshold_{verify_thres(threshold)}, elements_{capacity_} {}

    VecIt not_collision_detect(const KeyT &key);
    bool insert(const Element<KeyT, T> &elem);
    VecIt find(const KeyT &key);
    bool erase(const size_t pos);

    size_t size() const {
        return size_;
    }
    double threshold() const {
        return threshold_;
    }
    VecIt end() {
        return elements_.end();
    }
    VecIt begin() {
        return elements_.begin();
    }
    CVecIt cend() const {
        return elements_.cend();
    }
    CVecIt cbegin() const {
        return elements_.cbegin();
    }
};


template <typename KeyT, typename T>
typename Hashtable<KeyT, T>::VecIt Hashtable<KeyT, T>::not_collision_detect(const KeyT &key){
    for(size_t probe_num = 0; probe_num < capacity_; ++probe_num) {
        size_t pos = hash(key, probe_num);
        Element<KeyT, T> &vec_elem = elements_[pos];
        if(vec_elem.type != Ctrl::kFull || key == vec_elem.key)
            return elements_.begin() + pos;
    }
    return elements_.end();
}

template <typename KeyT, typename T>
bool Hashtable<KeyT, T>::insert(const Element<KeyT, T> &elem) {
    if ((static_cast<double>(size_) / capacity_) >= threshold_)
        resize();
    auto vec_it = not_collision_detect(elem.key);
    if(vec_it == elements_.end())
        return false;
    if(vec_it->type == Ctrl::kEmpty || vec_it->type == Ctrl::kDeleted) {
        *vec_it = elem;
        ++size_;
        return true;
    }
    return false;
}

template <typename KeyT, typename T>
typename Hashtable<KeyT, T>::VecIt Hashtable<KeyT, T>::find(const KeyT &key){
    auto vec_it = not_collision_detect(key);
    if(vec_it == elements_.end())
        return vec_it;
    if(vec_it->type == Ctrl::kFull)
        return vec_it;
    return elements_.end();
}

template <typename KeyT, typename T>
bool Hashtable<KeyT, T>::erase(const size_t pos) {
    if(pos >= capacity_) {
        return false;
    }
    Element<KeyT, T> &elem = elements_[pos];
    if (elem.type != Ctrl::kFull)
        return false;
    elem.type = Ctrl::kDeleted;
    --size_;
    return true;
}


//--------------------- private methods ------------------------------
template <typename KeyT, typename T>
double Hashtable<KeyT, T>::verify_thres(const double threshold) const{
    if(threshold >= 1.0) {
        std::cout << "Threshold should be < 1, the entered value was changed to the default: " << DEFAULT_THRESHOLD << std::endl;
        return DEFAULT_THRESHOLD;
    }
    if(threshold <= 0.0) {
        std::cout << "Threshold should be positive, the entered value was changed to the default: " << DEFAULT_THRESHOLD << std::endl;
        return DEFAULT_THRESHOLD;
    }
    return threshold;
}

template <typename KeyT, typename T>
size_t Hashtable<KeyT, T>::verify_cap(const size_t capacity) const{
    if(!std::has_single_bit(capacity)) {
        size_t new_cap = std::bit_ceil(capacity);
        std::cout << "Capacity isn't a power of two" << std::endl;
        std::cout << "Capacity was changed to the the smallest power of two that is not smaller than entered value: " << new_cap << std::endl;
        return new_cap;
    }
    return capacity;
}

template <typename KeyT, typename T>
void Hashtable<KeyT, T>::swap(Hashtable &other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(threshold_, other.threshold_);
    elements_.swap(other.elements_);
}

template <typename KeyT, typename T>
void Hashtable<KeyT, T>::fill_empty(VecIt elem, Element<KeyT, T> &to_insert) {
    if(elem == elements_.end() || elem->type != Ctrl::kEmpty)
        return;
    *elem = to_insert;
    ++size_;
}
template <typename KeyT, typename T>
void Hashtable<KeyT, T>::resize() {
    Hashtable tmp_tab(capacity_ * 2, threshold_);

    for(auto it : elements_) {
        if(it.type == Ctrl::kFull) {
            auto vec_it = tmp_tab.not_collision_detect(it.key);
            tmp_tab.fill_empty(vec_it, it);
        }
    }
    swap(tmp_tab);
}


} //namespace hashtable



#endif //__HASH_TABLE_H__