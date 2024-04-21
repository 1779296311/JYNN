
#pragma once

#include "facilities/traits.hpp"

namespace JYNN{

namespace Category{
    template <size_t dim>
    struct Dim {
        constexpr static size_t dim_ = dim;
    };

    using Scalar = Dim<0>;
    using Vector = Dim<1>;
    using Matrix = Dim<2>;
    struct OutOfTags;
};

// Valid  Category
template <class T>
constexpr  bool isValidCategory {false};
template <size_t dim>
constexpr  bool isValidCategory<Category::Dim<dim>> {true} ;

// decl
template <size_t Dim, class Item, class Device>
class Tensor;

// Principal Data Type
template <size_t Dim, class Item, class Device>
struct PrincipalDataType_ : 
    std::enable_if<true, Tensor<Dim, Item, Device>>
{};
template <class Category, class Item, class Device>
using  PrincipalDataType = 
    typename PrincipalDataType_<Category::dim_, Item, Device>::type;

// get category -> if not have -> return OutOfTags;
template <class T, class=void>
struct DataCategory_ : 
    std::enable_if<true, Category::OutOfTags>
{};
template <class T>
struct DataCategory_<T, std::void_t<typename T::CategoryTag>> :
        std::enable_if<true, typename T::CategoryTag>
{};
template <class T>
using DataCategory = typename DataCategory_<RmConstRef<T>>::type;

// others
template <class T, size_t Dim>
constexpr static bool isTensorWithDim = 
    std::is_same_v<DataCategory<T>, Category::Dim<Dim>>;

template <class T>
constexpr static bool isScalar  = isTensorWithDim<T, 0>;
template <class T>
constexpr static bool isVector  = isTensorWithDim<T, 1>;
template <class T>
constexpr static bool isMatrix  = isTensorWithDim<T, 2>;
template <class T>
constexpr static bool is3DArray = isTensorWithDim<T, 3>;

};