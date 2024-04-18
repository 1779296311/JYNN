#pragma once


#include "data/facilities/continuousMem.hpp"
#include "facilities/traits.hpp"
#include "data/facilities/device.hpp"


namespace JYNN {

namespace TensorInner{

template <class Shape, class Cur, class Next, class... Items>
auto OffsetAndVal(const Shape& shape, size_t &gap, 
                  Cur cur, Next next, Items... items) {

    constexpr size_t dim = Shape::dimSize_;
    constexpr size_t remain = sizeof...(items);
    constexpr size_t curIndex = dim - remain - 1;

    assert (
        shape[curIndex] > static_cast<size_t>(cur)
    );

    if constexpr (remain == 0) {
        return std::pair( static_cast<size_t>(cur), next );
    } else {
        auto [offset, val] = OffsetAndVal(shape, gap, next, items...);
        gap *= shape[curIndex + 1];
        return 
            std::pair(
                offset + gap * static_cast<size_t>(cur), 
                val
            );
    }
}


};

template <size_t Dim, class Item, class Device = Device::CPU>
class Tensor {
    static_assert( std::is_same_v<Item, RmConstRef<Item>> );
    static_assert( Dim > 0 );
private:
    JYNN::Shape<Dim> shape_;
    JYNN::ContinuousMem<Item, Device> mem_;

public:
    using CategoryTag = Category::Dim<Dim>;
    using ItemType = Item;
    using DeviceType = Device;

public:
    template <class... Dims>
    explicit Tensor(Dims... dims) : 
        shape_(dims...),
        mem_(shape_.Count())
    {};

    explicit Tensor(const Shape<Dim> &shape) :
        shape_(std::move(shape)),
        mem_(shape_.Count())
    {};

    explicit Tensor(Shape<Dim> shape, 
                    ContinuousMem<Item, Device> mem) :
        shape_(std::move(shape)),
        mem_(std::move(mem)) 
    {};


    const auto& GetShape() const {
        return shape_;
    }

    bool operator== (const Tensor &other) const {
        return ( shape_==other.shape_ && mem_==other.mem_);
    }
    bool WriteAvaliable () const {
        return !mem_.IsShared();
    }

    template <class... IndexsAndVal>
    void SetValue(IndexsAndVal... indexAndValue) {
        static_assert(std::is_same_v<DeviceType, JYNN::Device::CPU>);
        static_assert(sizeof...(indexAndValue) - 1 == Dim);
        assert( WriteAvaliable() );

        size_t gap = 1;
        auto [offset, val] = TensorInner::OffsetAndVal(shape_, gap, indexAndValue...);
        *(mem_.RawMemory() + offset) = static_cast<ItemType>(val);
    }
    template <class... Indexs> 
    ItemType operator()(Indexs... indexs) const {
        size_t offset = shape_.IndexToOffset(indexs...);
        return *(mem_.RawMemory() + offset);
    }

    const auto operator[] (size_t idx) const {
        assert(idx < shape_[0]);
        if constexpr (Dim > 1) {
            JYNN::Shape<Dim - 1> subShape;
            size_t count = 1;
            for(size_t i=1; i<Dim; ++i) {
                subShape[i-1] = shape_[i];
                count *= shape_[i];
            }
            size_t offset = count * idx;
            return Tensor<Dim - 1, ItemType, Device> (subShape, mem_.Shift(offset));
        } else{
            return Tensor<0, ItemType, Device>(mem_.Shift(idx));
        }

    }
};

template <class Item, class Device>
class Tensor<0, Item, Device> {
    static_assert( std::is_same_v<Item, RmConstRef<Item>> );
public:
    using CategoryTag = Category::Dim<0>;
    using ItemType = Item;
    using DeviceType = Device;

private:
    JYNN::ContinuousMem<Item, Device> mem_;

public:
    explicit Tensor (ItemType item = ItemType()) : mem_(1) {
        SetValue(item);
    }
    explicit Tensor(Shape<0>) : Tensor() 
    {};

    explicit Tensor(JYNN::ContinuousMem<Item, Device> mem) : mem_(std::move(mem)) {
        assert(mem_.Size() > 0);
    };

    const auto& GetShape() const noexcept {
        const static JYNN::Shape<0> shape;
        return shape;
    }

    bool WriteAvaliable () const {
        return !mem_.IsShared();
    }

    void SetValue(ItemType item) {
        assert( WriteAvaliable() );
        * mem_.RawMemory() = item;
    }
    ItemType Value() const noexcept {
        return * mem_.RawMemory();
    }

    bool operator== (const Tensor& other) const {
        return other.mem_ == mem_;
    }
    bool operator!= (const Tensor& other) const {
        return !(other.mem_ == mem_);
    }
};

template <size_t Dim, class Item, class Device>
struct LowerAccessImp< Tensor<Dim, Item, Device> > {
private:
    Tensor<Dim, Item, Device> data_;

public:
    LowerAccessImp(Tensor<Dim, Item, Device> tensor) : 
        data_( std::move(tensor) ) {}

    Item* MutableRawMemory() {
        return data_.mem_.RawMemory();
    }
    const Item* MutableRawMemory() const {
        return data_.mem_.RawMemory();
    }
    auto SharedMemory() {
        return data_.mem_;
    } 
};

template <class Item, class Device = Device::CPU>
using Scalar = Tensor<0, Item, Device>;

template <class Item, class Device = Device::CPU>
using Vector = Tensor<1, Item, Device>;

template <class Item, class Device = Device::CPU>
using Matrix = Tensor<2, Item, Device>;

template <class Item, class Device = Device::CPU>
using Tensor3D = Tensor<3, Item, Device>;


using VectorF = Vector<float>;
using ScalarF = Scalar<float>;
using MatrixF = Matrix<float>;
using Tensor3DF = Tensor3D<float>;


};