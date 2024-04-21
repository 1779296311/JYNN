#pragma once

#include "data/facilities/category.hpp"
#include "operation/facilities/instanceID.hpp"

namespace JYNN {



template <class...> struct OperandContainer;

/// @brief Check Valid
/// @tparam OpTag 
/// @tparam ...Operands 
template <class OpTag, class... Operands>
constexpr bool isValidOperand = (isValidCategory<DataCategory<Operands>> && ...);


/// @brief Get operand type
template <class OpTag, class Operand0, class... Operands>
struct OperItemType_ : 
    std::enable_if<true, typename Operand0::ItemType>
{};

template <class OpTag, class... Operands>
using OperItemType = typename OperItemType_<OpTag, Operands...>::type;

template <class OpTag, class Operand0, class... Operands>
struct OperDeviceType_ : 
    std::enable_if<true, typename Operand0::DeviceType>  
{};

template <class OpTag, class... Operands>
using OperDeviceType = typename OperDeviceType_<OpTag, Operands...>::type;

// deduce the res Category
template <class Category0, class... Categor1toN>
struct PickCommonCategory_ : 
    std::enable_if<true, Category0>
{};
template <class CategoryRes, class CategoryCur, class... Categories>
struct PickCommonCategory_<CategoryRes, CategoryCur, Categories...> : 
    std::conditional_t<
        (CategoryRes::dim_ > CategoryCur::dim_),
        PickCommonCategory_<CategoryRes, Categories...>,
        PickCommonCategory_<CategoryCur, Categories...>
    >
{};
template <class OpTag, class Policy, class... Categories>
struct OperCategoryDeduce_ : 
    PickCommonCategory_<Categories...>
{};

template <class OpTag, class Policy, class... Operands>
using OperCategoryDeduce = typename  
    OperCategoryDeduce_<OpTag, Policy, DataCategory<Operands>... >::type;

// operator auxiliary parametes (runtime)
template <class OpTag, class Item, class Category>
struct OperAuxParams{
    bool operator== (const OperAuxParams& ) const  {
        return true;
    }
};

// Shape
namespace OperShapeInfoInner {

template <class Shape>
auto CommonShape(const Shape& shape) {
    return shape;
}
template <class ShapeRes, class ShapeCur, class... Shapes>
auto CommonShape(const ShapeRes& res, const ShapeCur& cur, const Shapes&... shapes) {
    assert(IsBoardcastAble(res, cur));
    if constexpr (ShapeRes::dimSize_ < ShapeCur::dimSize_) {
        return CommonShape(cur, shapes...);
    }else{ 
        return CommonShape(res, shapes...);
    }
}

};
template <class Shape1, class Shape2>  
bool IsBoardcastAble(const Shape1& shape1, const Shape2& shape2) {
    if constexpr (!Shape1::dimSize_ || !Shape2::dimSize_) {
        return true;
    } else if constexpr (Shape1::dimSize_ < Shape2::dimSize_) {
        return IsBoardcastAble(shape2, shape1);
    } else {
        auto t1 = shape1.rbegin();
        auto t2 = shape2.rbegin();
        auto end2 = shape2.rend();
        while(t2 != end2) {
            if(*t1 != *t2) {
                return false;
            }
            ++t1;
            ++t2;
        }
        return true;
    }
}
template <class OpTag, class Policies, class Category>
struct OperShapeInfo{
private:
    Shape<Category::dim_> shape_;

public:
    template <class OperAuxParams, class... Operands>
    explicit OperShapeInfo (const OperAuxParams&, const Operands&... operands) : 
        shape_(OperShapeInfoInner::CommonShape(operands.GetShape() ...))
    {};
    const auto& GetShape() const {
        return shape_;
    }
};

// operator calculate
template <class... >
struct OperCalculateChina;

template <class OpTag> struct OperSeq_;



/// Oper Aux value
template <class T>
struct OperAuxValue {
private:
    T value_;
    size_t instID_;
public:
    OperAuxValue (T value) :
        value_(value),
        instID_(InstanceID::Get()) 
    {};

    const auto& Value() const {
        return value_;
    }
    bool operator== (const OperAuxValue& other) const {
        return instID_ == other.instID_;
    }
};



};