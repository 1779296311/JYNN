#pragma once

#include "facilities/contMetaFuns/sequential.hpp"
#include "policy/policyContainer.hpp"
#include "operation/facilities/organizer.hpp"
#include "operation/tensor/slice.hpp"


namespace JYNN{

namespace OpTags {
    struct Slice;
};

template <class Operand>
constexpr bool isValidOperand<OpTags::Slice, Operand> {DataCategory<Operand>::dim_ > 0};

template <class OpTag, class Operands, class Policies = PolicyContainer<>>
class Operation;


/************* Class Operation ***************************************************************************/
template <class OpTag, class... Operands, class TPolicies>
class Operation <OpTag, OperandContainer<Operands...>, TPolicies> {

    static_assert(sizeof...(Operands) > 0);
    static_assert((std::is_same_v<RmConstRef<Operands>, Operands> && ...));

public:
    using Policies = TPolicies;
// 这会调用 各个 tag 的 特化， 比如 Slice 就会调用 slice.hpp : OperCategoryDeduce_
    using CategoryTag = OperCategoryDeduce<OpTag, Policies, Operands...>;
    using ItemType = OperItemType<OpTag, Operands...>;
    using DeviceType = OperDeviceType<OpTag, Operands...>;

    template <size_t idx>
    using OperandType = Sequential::At<idx, OperandContainer<Operands...>>;

private:
    OperAuxParams <OpTag, ItemType, CategoryTag> auxParams_;
    OperShapeInfo <OpTag, Policies, CategoryTag> shapeInfo_;
    std::tuple<Operands...> operands_;

    using Principal = PrincipalDataType<CategoryTag, ItemType, DeviceType>;

public:
    explicit Operation(OperAuxParams<OpTag, ItemType, CategoryTag> params,
                       Operands... operands) : 
            auxParams_(std::move(params)),
            shapeInfo_(auxParams_, operands...), 
            operands_( {std::move(operands)...} )
    {};

    explicit Operation(Operands... operands) : 
        Operation(OperAuxParams <OpTag, ItemType, CategoryTag>{}, std::move(operands)...)
    {};

    template <size_t idx>
    const auto& Operand() const {
        return std::get<idx>(operands_);
    }

    const auto& OperandTuple() const {
        return operands_;
    }
    const auto& AuxParams() const {
        return auxParams_;
    }
    const auto& GetShape() const {
        return shapeInfo_.GetShape();
    }

    bool operator== (const Operation& other) {
        return  (auxParams_ == other.auxParams_) &&
                (operands_ == other.operands_);
    }

//Slice
    auto operator[] (size_t idx) const {
        static_assert(isValidOperand<OpTags::Slice, Operation>);
        using ResType = Operation<OpTags::Slice, OperandContainer<Operation>>;
        using Param0 = OperAuxParams<OpTags::Slice, 
                                     typename ResType::ItemType, 
                                     typename ResType::CategoryTag>;
        return ResType(Param0{idx}, (const Operation&)*this);
    }

};


};