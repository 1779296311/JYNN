#pragma once

#include "data/facilities/shape.hpp"

namespace JYNN {

template <size_t Dim, class Item, class Device = Device::CPU>
class ZeroTensor {
public:
    using CategoryTag = Category::Dim<Dim>;
    using ItemType = Item;
    using DeviceType = Device;

private:
    JYNN::Shape<Dim> shape_;

public:
    
    template <class... Indexs>
    explicit ZeroTensor (Indexs&&... index) : 
        shape_(std::forward<Indexs>(index)...)
    {};
    explicit ZeroTensor (const Shape<Dim>& shape) : 
        shape_(std::move(shape))
    {}
    bool operator== (const ZeroTensor& other) const {
        return (shape_ == other.shape_) ;
    }
    bool operator!= (const ZeroTensor& other) const {
        return !(*this == other);
    }
};


};