#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"

namespace JYNN::OpTags{
    struct Reshape;
};

namespace JYNN{
    constexpr static size_t InferredDim = (size_t)-1;

template <class Policy, class Cate>
class OperShapeInfo<OpTags::Reshape, Policy, Cate> {

private:
    JYNN::Shape<Cate::dim_> shape_;

public:
    template <class OperAuxParams, class Operand>
    OperShapeInfo(const OperAuxParams& auxParam, const Operand& op) :
        shape_(auxParam.shape_)
    {}

    const auto& GetShape() const {
        return shape_;
    }
};

template <typename Item, typename Cate>
struct OperAuxParams<OpTags::Reshape, Item, Cate> {
    template <typename T>
    OperAuxParams(JYNN::Shape<Cate::dim_> new_shape, const T& old_shape)
        : shape_(new_shape) {

        const size_t ori_shape_count = old_shape.Count();
        if constexpr(Cate::dim_ == 0) {
            assert(ori_shape_count == 1);
        } else {
            size_t inferred_pos = InferredDim;
            size_t current_count = 1;
            for (size_t i = 0; i < shape_.size(); ++i){
                if (shape_[i] == InferredDim){
                    assert(inferred_pos == InferredDim);
                    inferred_pos = i;
                } else {
                    current_count *= shape_[i];
                }
            }
            if ( inferred_pos == InferredDim ) {
                assert(current_count == ori_shape_count);
            } else {
                assert(ori_shape_count % current_count == 0);
                shape_[inferred_pos] = ori_shape_count / current_count;
            }
        }
    }
    JYNN::Shape<Cate::dim_> shape_;
};

template <class Policy, class Cate>
struct OperCategoryDeduce_<OpTags::Reshape, Policy, Cate>{
    constexpr static size_t dim = PolicySelect<DimPolicy, Policy>::DimCount;
    using type = Category::Dim<dim>;
};


template <class Operand, size_t dim,
          class = std::enable_if_t<isValidOperand<OpTags::Reshape, Operand>>
          >
auto Reshape(Operand&& op, JYNN::Shape<dim> shape) {

    using rawOp = RmConstRef<Operand>;
    using resType = Operation<
        OpTags::Reshape,
        OperandContainer<rawOp>,
        PolicyContainer<PDimCountIs<dim>>
    >;
    OperAuxParams<
        OpTags::Reshape,
        typename resType::ItemType,
        typename resType::CategoryTag
    > aux(std::move(shape), op.GetShape());
    return resType(std::move(aux), std::forward<Operand>(op));
}

};