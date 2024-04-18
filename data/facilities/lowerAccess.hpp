#pragma once

#include "facilities/traits.hpp"

namespace JYNN{

template <class T>
struct LowerAccessImp;

template <class Data>
auto LowerAccess(Data && data) {
    using dtype = RmConstRef<Data>;
    return LowerAccessImp<dtype>(std::forward<Data>(data));
}

};