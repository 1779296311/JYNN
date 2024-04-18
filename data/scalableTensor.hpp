#pragma once

#include <deque>
#include <memory>

#include "data/facilities/shape.hpp"
#include <algorithm>

namespace JYNN {

namespace ScalableTensorInner {

template<size_t Dim>
auto ShapeInitFrom(const Shape<Dim>& shape) {
    Shape<Dim + 1> res;
    if constexpr (0 != Dim) {
        std::copy(
            std::begin(shape),
            std::end(shape),
            std::begin(res) + 1
        );
    }
    res[0] = 0;
    return res;
}
};

template <class Tensor>
class ScalableTensor{
    static_assert( std::is_same_v<Tensor, RmConstRef<Tensor>> );

    static constexpr size_t tensorDim_ = Tensor::CategoryTag::dim_;

private:
    JYNN::Shape<tensorDim_ + 1> shape_;
    std::shared_ptr<std::deque<Tensor>> tensors_ {
        std::make_shared<std::deque<Tensor>>()
    };

public:
    using CategoryTag = Category::Dim<tensorDim_ + 1>;
    using ItemType = typename Tensor::ItemType;
    using DeviceType = typename Tensor::DeviceType;

public:

    explicit ScalableTensor() = default;

    template <class... Indexs,
              std::enable_if_t<(std::is_convertible_v<size_t, Indexs> && ...)>* = nullptr,
              std::enable_if_t<sizeof...(Indexs) == tensorDim_>* = nullptr
             >
    explicit ScalableTensor(Indexs&&... indexs) : 
        shape_(0, std::forward<Indexs>(indexs)...)
    {}

    explicit ScalableTensor(const Shape<tensorDim_>& shape) {
        std::copy(
            std::begin(shape),
            std::end(shape),
            std::begin(shape_) + 1
        );
        shape_[0] = 0;
    }

    template <class Iterator,
              std::enable_if_t<isIterator<Iterator>>* = nullptr
             >
    explicit ScalableTensor(Iterator l, Iterator r) {
        while(l != r) {
            PushBack(*l);
            l++;
        } 
    }

    void PushBack(const Tensor& tensor) {
        //assert( Tensor::CategoryTag::dim_ == tensorDim_);
        if (tensors_->empty()) {
            shape_ = ScalableTensorInner::ShapeInitFrom(tensor.GetShape());
        }
        if constexpr (tensorDim_) {
            assert (
                std::equal (
                    std::begin(shape_) + 1,
                    std::end(shape_),
                    std::begin(tensor.GetShape())
                )
            );
        }
        tensors_->push_back(std::move(tensor));
        ++shape_[0];
    }

    const auto& GetShape() const {
        return shape_;
    }

    bool WriteAvaliable() const {
        return tensors_.use_count() == 1;
    }
    void Clear() {
        assert( WriteAvaliable() );
        tensors_.clear();
        shape_[0] = 0;
    }
    void Reverse() {
        assert( WriteAvaliable() );
        if(!tensors_) {
            return;
        }
        auto &cont = *tensors_;
        std::reverse(std::begin(cont), std::end(cont));
    }

    bool Empty() const {
        return tensors_->empty();
    }

    const auto& operator[] (size_t idx) const {
        return (*tensors_)[idx];
    }
    bool operator== (const ScalableTensor& other) const {
        return tensors_ == other.tensors_;
    }
};

};