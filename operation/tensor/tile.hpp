#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"

namespace JYNN::OpTags{
    struct Tile;
};

namespace JYNN{

namespace TileInner{
    template <class TIndexes, class TDimArray>
    struct DimArrToBitHelper_;
    
    template <class TDimArray, size_t... I>
    struct DimArrToBitHelper_<std::index_sequence<I...>, TDimArray> {
        using type = PDimBitArrayIs<ValueSequential::Contains<TDimArray, I>...>;
    };
    
    template <size_t AimDim, class Policy>
    struct DimArrToBit_ {
        using DimArray = PolicyGet<Policy, DimPolicy, DimPolicy::DimArrayValueCate>;
        using type = typename DimArrToBitHelper_<std::make_index_sequence<AimDim>, DimArray>::type;
    };
    
    template <class TIndexes, size_t TrueDimBound>
    struct DefaultToBitHelper_;
    
    template <size_t TrueDimBound, size_t... I>
    struct DefaultToBitHelper_<std::index_sequence<I...>, TrueDimBound>{
        using type = PDimBitArrayIs<(I < TrueDimBound)...>;
    };
    
    template <size_t AimDim, size_t OriDim>
    struct DefaultToBit_ {
        using type = typename DefaultToBitHelper_<std::make_index_sequence<AimDim>, AimDim - OriDim>::type;
    };

    template <size_t AimDim, size_t uDim>
    constexpr bool CheckBound(std::array<size_t, uDim> dimArray){
        for (size_t i = 0; i < uDim; ++i){
            if (dimArray[i] >= AimDim){
                return false;
            }
        }
        return true;
    }

    template <size_t uAimDim, size_t uOriDim>
    auto GetMuteShape(const std::array<bool, uAimDim>& dimBitArray, const Shape<uOriDim>& shape) {
        Shape<uAimDim> resShape;
        
        size_t oriPos = 0;
        for (size_t aimPos = 0; aimPos < uAimDim; ++aimPos) {
            if (dimBitArray[aimPos]) {
                resShape[aimPos] = 1;
            } else {
                resShape[aimPos] = shape[oriPos++];
            }
        }
        assert(oriPos == uOriDim);
        return resShape;
    }

    template <size_t uDim>
    bool IsShapeCompatible(const Shape<uDim>& outShape, const Shape<uDim>& inShape){
        for (size_t i = 0; i < uDim; ++i) {
            if (outShape[i] % inShape[i] != 0) {
                return false;
            }
        }
        return true;
    }
};

template <typename Policy, typename Cate>
class OperShapeInfo<OpTags::Tile, Policy, Cate> {
public:
    template <typename OperAuxParams, typename Op>
    OperShapeInfo(const OperAuxParams& param, const Op& op)
        : shape_(param.GetShape())
    {}

    const auto& GetShape() const {
        return shape_;
    }

private:
    JYNN::Shape<Cate::dim_> shape_;
};

template <class Policy, class Operand>
struct OperCategoryDeduce_<OpTags::Tile, Policy, Operand> {
    constexpr static auto dimBitArray = PolicySelect<DimPolicy, Policy>::DimBitArray;
    using type = Category::Dim<dimBitArray.size()>;
};

template <class TElem, class Cate>
class OperAuxParams<OpTags::Tile, TElem, Cate> {
private:
    Shape<Cate::dim_> shape_;

public:
    OperAuxParams(Shape<Cate::dim_> aimShape)
        : shape_(std::move(aimShape))
    {}

    const auto& GetShape() const { 
        return shape_; 
    }

    bool operator == (const OperAuxParams& val) const {
        return shape_ == val.shape_;
    }
};

template <class Policy = PolicyContainer<>, class Operand, size_t AimDim,
          class = std::enable_if_t<isValidOperand<OpTags::Tile, Operand>>
         >
auto Tile(Operand&& oper, Shape<AimDim> aimShape) {
    static_assert(DataCategory<Operand>::dim_ <= AimDim);

    constexpr bool HasDimArray = PolicyExist<Policy, DimPolicy, DimPolicy::DimArrayValueCate>;
    if constexpr (HasDimArray) {
        constexpr auto dimArray = PolicySelect<DimPolicy, Policy>::DimArray;
        static_assert(DataCategory<Operand>::dim_ + dimArray.size() == AimDim, "Shape size mismatch");
    }

    using TDimBits = typename std::conditional_t<HasDimArray,
                                                    TileInner::DimArrToBit_<AimDim, Policy>,
                                                    TileInner::DefaultToBit_<AimDim, DataCategory<Operand>::dim_>>::type;
    assert(TileInner::IsShapeCompatible(aimShape,
                                        TileInner::GetMuteShape(TDimBits::DimBitArray, oper.GetShape())));

    using rawOp = RmConstRef<Operand>;
    using ResType = Operation<OpTags::Tile, OperandContainer<rawOp>, PolicyContainer<TDimBits>>;
    return ResType(OperAuxParams<OpTags::Tile,
                                    typename rawOp::ItemType,
                                    typename ResType::CategoryTag>(std::move(aimShape)),
                    std::forward<Operand>(oper));
}  

};