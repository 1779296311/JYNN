#pragma once


#include "operation/facilities/organizer.hpp"

namespace JYNN{

namespace OpTags {
    struct Slice;
};


template <class Policy, class Category>
struct OperCategoryDeduce_<OpTags::Slice, Policy, Category> : 
    std::enable_if< true, typename Category::Dim<Category::dim_ - 1> >
{};




template <class Item, class Category>
struct OperAuxParams<OpTags::Slice, Item, Category> {

    const size_t itemId_;

    OperAuxParams(size_t itemId) : 
        itemId_(itemId) 
    {};
    
    bool operator==(const OperAuxParams& other) const {
        return itemId_ == other.itemId_;
    }
};


// Shape
template <class Policies, class Category>
struct OperShapeInfo <OpTags::Slice, Policies, Category>{
private:
    Shape<Category::dim_> shape_;

public:
    template <class OperAuxParams, class Operand>
    explicit OperShapeInfo (const OperAuxParams&, const Operand& operand) {

        static_assert(Category::dim_ + 1 == DataCategory<Operand>::dim_);
        if constexpr (Category::dim_ != 0) {
            const auto& fshape = operand.GetShape();
            for(size_t i=0; i<Category::dim_; ++i) {
                shape_[i] = fshape[i+1];
            }
        }
    };


    const auto& GetShape() const {
        return shape_;
    }
};


};