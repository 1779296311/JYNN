#pragma once
#include "data/facilities/category.hpp"
#include "facilities/traits.hpp"

#include <iterator>

namespace JYNN{


// check if a iterator
template <class T, class = void>
constexpr static bool isIterator {false};
template <class T>
constexpr static bool isIterator
        <T, std::void_t<typename std::iterator_traits<T>::iterator_category>> {true};

// operator class that has CategoryTag ;
template <class A, class B,
          std::void_t<typename A::CategoryTag>* = nullptr,
          std::void_t<typename B::CategoryTag>* = nullptr,
          std::enable_if_t<!std::is_same_v<A, B>>* = nullptr
         >
bool operator== (const A& a, const B& b) {
    return false;
}

template <class A, class B,
          std::void_t<typename A::CategoryTag>* = nullptr,
          std::void_t<typename B::CategoryTag>* = nullptr
         >
bool operator!= (const A& a, const B& b) {
    return !(a == b);
}

};