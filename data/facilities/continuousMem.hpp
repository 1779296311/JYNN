#pragma once

#include "facilities/traits.hpp"
#include "data/facilities/allocator.hpp"

namespace JYNN{

template <class Item, class Device>
class ContinuousMem {
private:
    static_assert(
        std::is_same_v<
            Item,
            RmConstRef<Item>
        >
    );
    using ItemType = Item;

private:
    std::shared_ptr<ItemType> mem_ {nullptr};
    size_t size_ {0};

private:
    ContinuousMem(std::shared_ptr<ItemType> ptr, size_t itemN) : 
        mem_(std::move(ptr)),
        size_(itemN)
    {}

public:
    explicit ContinuousMem(size_t itemN) : 
        mem_(Allocator<Device>::template Allocate<ItemType>(itemN)), 
        size_(itemN)
    {}

    ContinuousMem Shift(size_t pos) const {
        assert(size_ > pos);
        return ContinuousMem(std::shared_ptr<ItemType>(mem_, mem_.get()+pos), size_ - pos);
    }

    bool IsShared() const {
        return mem_.use_count() > 1;
    }

    auto RawMemory() const {
        return mem_.get();
    }

    size_t Size() const {
        return size_;
    }

    bool operator== (const ContinuousMem& other) const {
        return (mem_ == other.mem_) && (size_ == other.size_);
    }
    bool operator!= (const ContinuousMem& other) const {
        return !(*this == other);
    }
};

};