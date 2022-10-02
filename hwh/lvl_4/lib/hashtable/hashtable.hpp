#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <vector>
#include <utility>
#include <bit>
#include <cmath>

#define DEFAULT_THRESHOLD 0.75

namespace hashtable {

enum Ctrl {
    kFull,           
    kEmpty,
    kDeleted
};

template <typename KeyT, typename T>
struct Element {
    KeyT key{};
    T data{};
    Ctrl type = kEmpty;

    Element() {}
    Element(std::pair<KeyT, T> pair): key(pair.first), data(pair.second), type(kFull) {}
};

template <typename KeyT, typename T>
class Hashtable {

    using VecOfElems = typename std::vector<Element<KeyT, T>>;
    using VecIt = typename VecOfElems::iterator;
    using CVecIt = typename VecOfElems::const_iterator;

    VecOfElems elements_;
    size_t size_ = 0;
    size_t capacity_;
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

public:

    Hashtable(size_t capacity, double threshold) {
        capacity_ = std::popcount(capacity) == 1 ? capacity : std::pow(2, static_cast<size_t>(std::ceil(log2(capacity))));
        threshold_ = (threshold < 1.0 && threshold > 0.0) ? threshold : DEFAULT_THRESHOLD;
        
        VecOfElems tmp_vec{capacity_};
        elements_.swap(tmp_vec);
    }
    bool insert(Element<KeyT, T> &elem);
    VecIt find(const KeyT &key);
    bool erase(size_t pos);
    VecIt check_table(const KeyT &key);

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
typename Hashtable<KeyT, T>::VecIt Hashtable<KeyT, T>::check_table(const KeyT &key){
    for(size_t probe_num = 0; probe_num < capacity_; ++probe_num) {
        size_t pos = hash(key, probe_num);
        Element<KeyT, T> &vec_elem = elements_[pos];
        if((vec_elem.type == kFull && key == vec_elem.key) || vec_elem.type == kEmpty)
            return elements_.begin() + pos;
    }
    return elements_.end();
}

template <typename KeyT, typename T>
bool Hashtable<KeyT, T>::insert(Element<KeyT, T> &elem) {
    if ((static_cast<double>(size_) / capacity_) >= threshold_)
        resize();
    auto vec_it = check_table(elem.key);
    if(vec_it != elements_.end()) {
        if(vec_it->type == kEmpty) {
            *vec_it = elem;
            ++size_;
            return true;
        }
        return false;
    }
    return false;
}


template <typename KeyT, typename T>
typename Hashtable<KeyT, T>::VecIt Hashtable<KeyT, T>::find(const KeyT &key){
    auto vec_it = check_table(key);
    if(vec_it != elements_.end()) {
        if(vec_it->type == kFull)
            return vec_it;
        return elements_.end();
    }
    return elements_.end();
}

template <typename KeyT, typename T>
void Hashtable<KeyT, T>::swap(Hashtable &other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(threshold_, other.threshold_);
    elements_.swap(other.elements_);
}

template <typename KeyT, typename T>
void Hashtable<KeyT, T>::resize() {
    Hashtable tmp_tab(capacity_ * 2, threshold_);

    for(auto it : elements_) {
        if(it.type == kFull) {
            auto vec_it = tmp_tab.check_table(it.key);
            if(vec_it != tmp_tab.elements_.end()) {
                if(vec_it->type == kEmpty) {
                    *vec_it = it;
                    ++tmp_tab.size_;
                }
            }
        }
    }
    swap(tmp_tab);
}

template <typename KeyT, typename T>
bool Hashtable<KeyT, T>::erase(size_t pos) {
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