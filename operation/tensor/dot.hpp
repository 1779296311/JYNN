#pragma once

#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"

namespace JYNN::OpTags{
    struct Dot;
};

namespace JYNN{

template <class Operand1, class Operand2>
constexpr bool isValidOperand<OpTags::Dot, Operand1, Operand2> {
    DataCategory<Operand1>::dim_ >= 1 &&
    DataCategory<Operand2>::dim_ >= 1
};

template <class Policy, class Category1, class Category2>
struct OperCategoryDeduce_<OpTags::Dot, Policy, Category1, Category2> :
    std::enable_if< true,
        Category::Dim<
            Category1::dim_ + Category2::dim_ - 
            2 * PolicySelect<DimPolicy, Policy>::ModifyDimNum
        >
    >
{};

template <class Policy, class Category>
struct OperShapeInfo<OpTags::Dot, Policy, Category> {
private:
    JYNN::Shape<Category::dim_> shape_;

public:
    template <class OperAuxParams, class Operand1, class Operand2>
    explicit OperShapeInfo (const OperAuxParams&, 
                            const Operand1& operand1, 
                            const Operand2& operand2) {
        
        if constexpr (Category::dim_ > 0) {
            constexpr size_t modifyDim = PolicySelect<DimPolicy, Policy>::ModifyDimNum;
            constexpr size_t dim1 = DataCategory<Operand1>::dim_;
            constexpr size_t dim2 = DataCategory<Operand2>::dim_;

            const auto& shape1 = operand1.GetShape();
            const auto& shape2 = operand2.GetShape();
            size_t p = 0;
            for(size_t i=0; i<dim1 - modifyDim; ++i) {
                shape_[p++] = shape1[i];
            }

            for(size_t i=modifyDim; i<dim2; ++i) {
                shape_[p++] = shape2[i];
            }
        }
    };

    const auto& GetShape() const {
        return shape_;
    }
};


template <class Policy=PolicyContainer<>,
          class Operand1, class Operand2,
          class=std::enable_if_t<isValidOperand<Operand1, Operand2>>
          >
auto Dot(Operand1&& op1, Operand2&& op2) {
    constexpr size_t modifyDim = PolicySelect<DimPolicy, Policy>::ModifyDimNum;
    constexpr size_t dim1 = DataCategory<Operand1>::dim_;
    constexpr size_t dim2 = DataCategory<Operand2>::dim_;

    static_assert(dim1 >= modifyDim && dim2 >= modifyDim);

    const auto& shape1 = op1.GetShape();
    const auto& shape2 = op2.GetShape();
    size_t p1 = dim1 - modifyDim;
    size_t p2 = 0;
    for(; p2<modifyDim; ++p1, ++p2) {
        assert(shape1[p1] == shape2[p2]);
    }

    using ResType = Operation<
        OpTags::Dot,
        OperandContainer<RmConstRef<Operand1>, RmConstRef<Operand2>>,
        PolicyContainer<PModifyDimNumIs<modifyDim>>
    >;
    return ResType(
        std::forward<Operand1>(op1),
        std::forward<Operand2>(op2)
    );
}

};