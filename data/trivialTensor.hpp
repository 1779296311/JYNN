#pragma once

#include "data/facilities/shape.hpp"

namespace JYNN{

template <size_t Dim, class Scalar>
class TrivialTensor{
public:
    using CategoryTag = Category::Dim<Dim>;
    using ItemType = typename Scalar::ItemType;
    using DeviceType = typename Scalar::DeviceType;

private:
    JYNN::Shape<Dim> shape_;
    Scalar scalar_;

public:

    template <class... Dims>
    explicit TrivialTensor(Scalar val, Dims... dims) : 
        shape_(dims...),
        scalar_(std::move(val))
    {}

    const auto& GetShape() const noexcept {
        return shape_;
    }

    bool operator== (const TrivialTensor& other) const {
        return (scalar_ == other.scalar_) && 
               (shape_ == other.shape_) ;
    }
    bool operator!= (const TrivialTensor& other) const {
        return !(*this == other);
    }
    const auto& Value() const {
        return scalar_;
    }
};

template <class Scalar, class... Dims>
explicit TrivialTensor(Scalar, Dims...) -> TrivialTensor<sizeof...(Dims), Scalar>;

};