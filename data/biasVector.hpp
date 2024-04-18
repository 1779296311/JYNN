#pragma once

#include "data/facilities/shape.hpp"

namespace JYNN {

template <class Scalar>
class BiasVector{
public:
    using CategoryTag = Category::Dim<1>;
    using ItemType = typename Scalar::ItemType;
    using DeviceType = typename Scalar::DeviceType;

private:
    JYNN::Shape<1> shape_;
    Scalar bias_;
    size_t biasIdx_;

public:
    explicit BiasVector (size_t len, Scalar scalar, size_t idx) : 
        shape_(len), bias_( std::move(scalar) ), biasIdx_(idx)
    {};
    const auto& GetShape() const {
        return shape_;
    }
    bool operator== (const BiasVector& other) const {
        return (bias_ == other.bias_) && 
               (shape_ == other.shape_) &&
               (biasIdx_ == other.biasIdx_) ;
    }
    bool operator!= (const BiasVector& other) const {
        return !(*this == other);
    }
    const auto& Value() const {
        return bias_;
    }
    size_t BiasIdx() const {
        return biasIdx_;
    }

};


};