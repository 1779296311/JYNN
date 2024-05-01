#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"

namespace JYNN::OpTags{
    struct ReduceSum;
};



namespace JYNN{

namespace ReduceSumInner{
    template <typename TPDim, typename TIndexes>
    struct DimArrToBitHelper_;
    
    template <typename TPDim, size_t... I>
    struct DimArrToBitHelper_<TPDim, std::index_sequence<I...>> {
        using type = PDimBitArrayIs<ValueSequential::Contains<TPDim, I>...>;
    };

    template <typename TPolicy, size_t uDimNum>
    struct DimArrToBit_ {
        using PDim = PolicyGet<TPolicy, DimPolicy, DimPolicy::DimArrayValueCate>;
        using type = typename DimArrToBitHelper_<PDim, std::make_index_sequence<uDimNum>>::type;
    };

    template <size_t uTrueBound, typename TIndexes>
    struct ModDimToBitHelper_;
    
    template <size_t uTrueBound, size_t... I>
    struct ModDimToBitHelper_<uTrueBound, std::index_sequence<I...>> {
        using type = PDimBitArrayIs<(I < uTrueBound)...>;
    };

    template <typename TPolicy, size_t uDimNum>
    struct ModDimToBit_ {
        constexpr static size_t ModDimNum = PolicySelect<DimPolicy, TPolicy>::ModifyDimNum;
        static_assert(ModDimNum <= uDimNum);
        using type = typename ModDimToBitHelper_<ModDimNum, std::make_index_sequence<uDimNum>>::type;
    };

    template <typename TIndexes>
    struct DefaultToBitHelper_;
    
    template <size_t... I>
    struct DefaultToBitHelper_<std::index_sequence<I...>> {
        using type = PDimBitArrayIs<(I >= 0)...>;
    };

    template <size_t uDimNum>
    struct DefaultToBit_ {
        using type = typename DefaultToBitHelper_<std::make_index_sequence<uDimNum>>::type;
    };

    template <size_t uDimNum>
    constexpr bool IsTrivialDimBits(const std::array<bool, uDimNum>& dimBits) {
        for (size_t i = 0; i < uDimNum; ++i)
        {
            if (dimBits[i]) return false;
        }
        return true;
    }

    template <size_t uDimNum>
    constexpr size_t AccuDimBits(const std::array<bool, uDimNum>& dimBits) {
        size_t res = 0;
        for (size_t i = 0; i < uDimNum; ++i)
        {
            if (dimBits[i]) ++res;
        }
        return res;
    }
    
    template <template<auto...> class Cont, auto DimBits, typename>
    struct DimBits2VaridicTemp_;
    
    template <template<auto...> class Cont, auto DimBits, int... Is>
    struct DimBits2VaridicTemp_<Cont, DimBits, Helper::IndexSequence<Is...>> {
        using type = Cont<DimBits[Is]...>;
    };
};


template <class Policies, class Cate>
class OperShapeInfo<OpTags::ReduceSum, Policies, Cate> {

private:
    JYNN::Shape<Cate::dim_> shape_;

public:
    template <class OperAuxParams, class Operand>
    OperShapeInfo(const OperAuxParams&, const Operand& op) {
        if constexpr (Cate::dim_) {
            constexpr auto& dimBits = PolicySelect<DimPolicy, Policies>::DimBitArray;
            auto& pshape = op.GetShape();

            if constexpr (PolicySelect<DimPolicy, Policies>::IsKeepDim) {
                for(size_t i=0; i<dimBits.size(); ++i) {
                    shape_[i] = dimBits[i] ? 1 : pshape[i];
                }
            } else {
                size_t p = 0;
                for(size_t i=0; i<dimBits.size(); ++i) {
                    if( !dimBits[i]) {
                        shape_[p++] = pshape[i];
                    }
                }
                assert(p == Cate::dim_);
            }
        }
    }

    const auto& GetShape() const {
        return shape_;
    }
};

template <class Policy, class Cate>
struct OperCategoryDeduce_<OpTags::ReduceSum, Policy, Cate> {
    constexpr static size_t oriDim = Cate::dim_;
    constexpr static auto dimBits = PolicySelect<DimPolicy, Policy>::DimBitArray;
    constexpr static size_t accDims = 
        PolicySelect<DimPolicy, Policy>::IsKeepDim ? 
        0 :
        ReduceSumInner::AccuDimBits(dimBits);
    using type = Category::Dim<oriDim - accDims>;
};


template <class Operand>
constexpr bool isValidOperand<OpTags::ReduceSum, Operand> {
    isValidCategory<DataCategory<Operand>> &&
    DataCategory<Operand>::dim_ > 0
};


template <class Policy=PolicyContainer<>, class Operand,
          class=std::enable_if<isValidOperand<OpTags::ReduceSum, Operand>>>
auto ReduceSum(Operand&& op) {

    constexpr bool hasDimArray = PolicyExist<Policy, DimPolicy, DimPolicy::DimArrayValueCate>;
    constexpr bool hasModDimNum = PolicyExist<Policy, DimPolicy, DimPolicy::ModifyDimNumValueCate>;

    static_assert( !(hasDimArray&&hasModDimNum) );

    using dimBits = 
        typename CompileTimeSwitch<
            std::integer_sequence<bool, hasDimArray, hasModDimNum> ,
            std::tuple<
                ReduceSumInner::DimArrToBit_<Policy, DataCategory<Operand>::dim_>,
                ReduceSumInner::ModDimToBit_<Policy, DataCategory<Operand>::dim_>,
                ReduceSumInner::DefaultToBit_<DataCategory<Operand>::dim_>
            >
        >::type;

    static constexpr bool keepDim = PolicySelect<DimPolicy, Policy>::IsKeepDim;
    constexpr auto dimBitArr = dimBits::DimBitArray;
    if constexpr (ReduceSumInner::IsTrivialDimBits(dimBitArr)) {
        return std::forward<Operand> (op);
    } else {
        using rawOp = RmConstRef<Operand>;
        using resType = Operation<
            OpTags::ReduceSum,
            OperandContainer<rawOp>,
            PolicyContainer<dimBits, PKeepDimValueIs<keepDim>>
        >;
        return resType(std::forward<Operand> (op));
    }
}

};