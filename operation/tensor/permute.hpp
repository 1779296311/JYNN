#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"

namespace JYNN::OpTags{
    struct Permute;
};

namespace JYNN{

template <class Operand>
constexpr bool isValidOperand<OpTags::Permute, Operand> 
    {isValidCategory<DataCategory<Operand>> && DataCategory<Operand>::dim_ > 1};

template <class Policies, class Cate>
class OperShapeInfo<OpTags::Permute,  Policies, Cate>{

private:
   JYNN::Shape<Cate::dim_> shape_;

public:
    template <class OperAuxParams, class Operand>
    OperShapeInfo(const OperAuxParams&, const Operand& operand) {

        constexpr auto dims = PolicySelect<DimPolicy, Policies>::DimArray;
        auto pshape = operand.GetShape();

        for(size_t i=0; i<Cate::dim_; ++i) {
            shape_[i] = pshape[dims[i]];
        }
    }

    const auto& GetShape() const {
        return shape_;
    }

};

namespace PermuteInner{

template <size_t Dims>
constexpr bool ValidPermute(const std::array<size_t, Dims>& dims) {
    std::array<bool, Dims> valid_ {};
    for(size_t i=0; i<Dims; ++i) {
        if(dims[i] >= Dims) {
            return false;
        }
        valid_[i] = true;
    }

    for(size_t i=0; i<Dims; ++i) {
        if(valid_[i] != true) {
            return false;
        }
    }
    return true;
}

template <size_t Dims>
constexpr bool TrivialDims(const std::array<size_t, Dims>& dims) {
    for(size_t i=0; i<Dims; ++i) {
        if(dims[i] != i) {
            return false;
        }
    }
    return true;
}

};


template <class Policies, class Operand,
          class=std::enable_if<isValidOperand<OpTags::Permute, Operand>>>
auto Permute(Operand&& op) {

    constexpr auto dims = PolicySelect<DimPolicy, Policies>::DimArray;
    static_assert(PermuteInner::ValidPermute(dims));

    if constexpr (PermuteInner::TrivialDims(dims)) {
        return std::forward<Operand>(op);
    } else {
        using rawOp = RmConstRef<Operand>;
        using Dim = PolicyGet<Policies, DimPolicy, DimPolicy::DimArrayValueCate>;
        using resType = Operation<
            OpTags::Permute,
            OperandContainer<rawOp>,
            PolicyContainer<Dim>
        >;
        return resType(std::forward<Operand>(op));
    }
}


template <class Operand,
          class=std::enable_if<isValidOperand<OpTags::Permute, Operand>> >
auto Transpose (Operand&& op){
    static_assert(DataCategory<Operand>::dim_ == 2);
    return Permute<PolicyContainer<PDimArrayIs<1, 0>>>(std::forward<Operand>(op));
}


namespace PermuteInvInner{

template <class IndexSeq, class DimArray>
struct DimInv_;

template <size_t... Index, class DimArray>
struct DimInv_<std::integer_sequence<size_t, Index...>, DimArray> :
    std::enable_if<true, PDimArrayIs<ValueSequential::Order<DimArray, Index>...>>
{};
}

template <class Policies, class Operand,
          class=std::enable_if<isValidOperand<OpTags::Permute, Operand>>>
auto PermuteInv(Operand&& op) {

    constexpr auto& dims = PolicySelect<DimPolicy, Policies>::DimArray;
    static_assert(PermuteInner::ValidPermute(dims));

    if constexpr (PermuteInner::TrivialDims(dims)) {
        return std::forward<Operand>(op);
    } else {
        using Dim = PolicyGet<Policies, DimPolicy, DimPolicy::DimArrayValueCate>;
        using modDim = 
            typename PermuteInvInner::DimInv_<std::make_index_sequence<dims.size()>, Dim>::type;
        return Permute<PolicyContainer<modDim>, Operand>(std::forward<Operand>(op));
    }
}

};


