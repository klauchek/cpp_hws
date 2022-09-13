#include <unordered_map>
#include <list>
#include <initializer_list>
#include <iostream>

struct S {
    const char* str;
};

void foo(std::unordered_map<int, std::list<S>> &map) {
    S a, b, c;
    a.str = "abcde";
    b.str = "abcdf";
    c.str = "abcdg";
    // S* st_a = &a;
    // S* st_b = &b;
    // S* st_c = &c;

    auto res = map.emplace(1, std::list<S>{std::initializer_list<S>{a}});
    std::cout << res.second << std::endl;

    res = map.emplace(1, std::list<S>{std::initializer_list<S>{b}});
    std::cout << res.second << std::endl;

    if(!res.second) {
        //auto elem = map.find(1);
        res.first->second.push_front(b);
    }

    res = map.emplace(1, std::list<S>{std::initializer_list<S>{c}});
    std::cout << res.second << std::endl;
    if(!res.second) {
        res.first->second.push_front(c);
    }
}

int main() {


    std::unordered_map<int, std::list<S>> map;
    // S a, b;
    // a.str = "abcde";
    // b.str = "abcdf";
    // S* st_a = &a;
    // S* st_b = &b;

    // auto res = map.emplace(1, std::list<S*>{std::initializer_list<S*>{st_a}});
    // std::cout << res.second << std::endl;
    // res = map.emplace(1, std::list<S*>{std::initializer_list<S*>{st_b}});
    // std::cout << res.second << std::endl;

    // if(!res.second) {
    //     res.first->second.push_front(st_b);
    // }
    foo(map);

    for(auto &v: map) {
        //std::cout << v.second.size() << std::endl;
        for(auto &u : v.second){
            std::cout << u.str << std::endl;
            //exit(0);
        }
    }
}