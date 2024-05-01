#pragma once

#include <array>
#include <numeric>

#include "facilities/contMetaFuns/helper.hpp"

namespace JYNN{

namespace ShapeInner{

/////////////////// 
template <size_t Dim, class CurIndex, class... Indexs>
size_t IndexToOffset(const std::array<size_t, Dim>& dims, size_t &gap, 
                    CurIndex cur, Indexs... indexs) {

    constexpr size_t dimIndex = Dim - sizeof...(indexs) - 1;
    assert (static_cast<size_t>(cur) < dims[dimIndex]);
    if constexpr (!sizeof...(indexs)) {
        return static_cast<size_t>(cur);
    } else {
        size_t res = IndexToOffset(dims, gap, indexs...);
        gap *= dims[dimIndex + 1];
        return res + (static_cast<size_t>(cur) * gap);
    }
}
template <size_t Dim, size_t... N>
size_t IndexToOffset(const std::array<size_t, Dim>& dims,
                    const std::array<size_t, Dim>& indexs,
                    Helper::IndexSequence<N...>*) {
    size_t gap = 1;
    return IndexToShift(dims, gap, std::get<N>(indexs)...);
}
//////////////////
template <size_t Index, class Shape>
void FillShape(Shape &shape) {
    return ;
}
template <size_t Index, class Shape, class Cur, class... Items>
void FillShape(Shape &shape, Cur cur, Items... items) {
    shape[Index] = static_cast<size_t>(cur);
    FillShape<Index+1>(shape, items...);
}
template <class Shape, class... Items>
void FillDims(Shape &shape, Items... items) {
    FillShape<0>(shape, items...);
}
};


template <size_t Dim>
class Shape{
    std::array<size_t, Dim> dims_;

public:
    constexpr static size_t dimSize_ = Dim;

    explicit Shape () = default;

    template <class... IndexTypes,
              std::enable_if_t<(std::is_convertible_v<size_t, IndexTypes> && ...)>* = nullptr
        >
    explicit Shape(IndexTypes... dims){
        static_assert(sizeof...(dims) == dimSize_);
        ShapeInner::FillDims(dims_, dims...);
    }
    bool operator== (const Shape& other) const {
        return other.dims_ == dims_;
    }
    bool operator!= (const Shape& other) const {
        return (other == *this);
    }
    size_t Count() const {
        return std::accumulate(
            std::begin(dims_),
            std::end(dims_),
            static_cast<size_t>(1),
            std::multiplies<>()
        );
    }

    template <class... IndexTypes,
              std::enable_if_t<(std::is_convertible_v<size_t, IndexTypes> && ...)>* = nullptr>
    size_t IndexToOffset(IndexTypes... indexs) const {
        static_assert(sizeof...(indexs) == dimSize_);
        size_t gap = 1;
        return ShapeInner::IndexToOffset(dims_, gap, indexs...);
    }

    size_t IndexToOffset(const std::array<size_t, dimSize_>& indexs) const {
        using indexsSeq = Helper::MakeIndexSequence<dimSize_>;
        return 
            ShapeInner::IndexToOffset(dims_, indexs, static_cast<indexsSeq*>(nullptr));
    }

    std::array<size_t, dimSize_> OffsetToIndex(size_t offset) const {

        std::array<size_t, dimSize_> res;
        for(int i=static_cast<int>(dimSize_)-1; i>=0; --i) {
            res[i] = offset % dims_[i];
            offset = (offset - res[i]) / dims_[i];
        }
        assert(offset == 0);
        return res;
    }

    void ShiftIndex(std::array<size_t, dimSize_>& indexs, int carry=1) const {
        assert(carry >= 0);
        size_t c = static_cast<size_t>(carry);
        int e = static_cast<int>(dimSize_);
        for(int i=e-1; i>=0 && c; --i) {
            indexs[i] += c;
            c = indexs[i] / dims_[i];
            indexs[i] %= dims_[i];
        }
        assert(c == 0);
    }

    size_t operator[] (size_t idx) const {
        assert(idx < dimSize_);
        return dims_[idx];
    }
    size_t& operator[] (size_t idx) {
        assert(idx < dimSize_);
        return dims_[idx];
    }

    auto size() const {
        return dims_.size();
    }

    auto begin() const {
        return dims_.begin();
    }

    auto begin() {
        return dims_.begin();
    }


    decltype(auto) end() const {
        return dims_.end();
    }
    decltype(auto) end() {
        return dims_.end();
    }
    decltype(auto) rbegin() const {
        return dims_.rbegin();
    }
    decltype(auto) rbeign() {
        return dims_.rbegin();
    } 

    decltype(auto) rend() const {
        return dims_.rend();
    }
    decltype(auto) rend() {
        return dims_.rend();
    }
};

template <>
class Shape<0>{
public:
    constexpr static size_t dimSize_ = 0;
    explicit Shape() = default;

    bool operator== (const Shape &other) const {
        return true;
    }
    template <size_t Dim>
    bool operator== (const Shape<Dim>&) const {
        return false;
    }
    size_t Count() const {
        return 1;
    }
    size_t size() const {
        return 0;
    }
};

template <size_t Dim1, size_t Dim2>
bool operator!= (const Shape<Dim1> &shape1, const Shape<Dim2> &shape2) {
    return !( shape1 == shape2 );
}

template <class... IndexTypes,
          std::enable_if_t<(std::is_convertible_v<size_t, IndexTypes> && ...)>* = nullptr  
         >
explicit Shape(IndexTypes...) -> Shape<sizeof...(IndexTypes)>;

};