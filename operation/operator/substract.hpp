#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"


namespace JYNN::OpTags{
    struct Substract;
    struct SubstractFromNum;
    struct SubstractByNum;
};


namespace JYNN{

namespace SubstractFromNumInner{

template <class Number, class Operand>
constexpr bool Valid() {
    if constexpr ((isValidCategory<DataCategory<Number>>) || (!isValidCategory<DataCategory<Operand>>)) {
        return false;
    } else {
        return std::is_constructible_v<typename RmConstRef<Operand>::ItemType, Number>;
    }
}
};

template <class Number, class Operand>
constexpr bool isValidOperand<OpTags::SubstractFromNum, Number, Operand> {
    SubstractFromNumInner::Valid<Number, Operand>()
};



template <class Item, class Cate>
struct OperAuxParams<OpTags::SubstractFromNum, Item, Cate> : public OperAuxValue<Item> {
    using Base = OperAuxValue<Item>;
    using Base::Base;
    using Base::operator=;
};


namespace SubstractByNumInner{

template <class Operand, class Number>
constexpr bool Valid() {
    if constexpr ((isValidCategory<DataCategory<Number>>) || (!isValidCategory<DataCategory<Operand>>)) {
        return false;
    } else {
        return std::is_constructible_v<typename RmConstRef<Operand>::ItemType, Number>;
    }
}
};

template <class Operand, class Number>
constexpr bool isValidOperand<OpTags::SubstractByNum,  Operand, Number> {
    SubstractByNumInner::Valid<Operand, Number>()
};

template <class Item, class Cate>
struct OperAuxParams<OpTags::SubstractByNum, Item, Cate> : public OperAuxValue<Item> {
    using Base = OperAuxValue<Item>;
    using Base::Base;
    using Base::operator=;
};

template <class Op1, class Op2,
          class = std::enable_if_t<
            isValidOperand<OpTags::Substract, Op1, Op2> ||
            isValidOperand<OpTags::SubstractFromNum, Op1, Op2> ||
            isValidOperand<OpTags::SubstractByNum, Op1, Op2>
          >
         >
auto operator- (Op1&& op1, Op2&& op2) {

    if constexpr (isValidOperand<OpTags::Substract, Op1, Op2>) {
        using rawOp1 = RmConstRef<Op1>;
        using rawOp2 = RmConstRef<Op2>;
        using resType = Operation<
            OpTags::Substract,
            OperandContainer<rawOp1, rawOp2>
        >;
        return resType(std::forward<Op1>(op1), std::forward<Op2>(op2));

    } else if constexpr (isValidOperand<OpTags::SubstractFromNum, Op1, Op2>) {

        using rawOp2 = RmConstRef<Op2>;
        using resType = Operation<
            OpTags::SubstractFromNum,
            OperandContainer<rawOp2>
        >;
        return resType(
            OperAuxParams<
                OpTags::SubstractFromNum,
                typename resType::ItemType,
                typename resType::CategoryTag
            > (op1),
            std::forward<Op2>(op2)
        );

    } else if constexpr (isValidOperand<OpTags::SubstractByNum, Op1, Op2>) {
        using rawOp1 = RmConstRef<Op1>;
        using resType = Operation<
            OpTags::SubstractByNum,
            OperandContainer<rawOp1>
        >;
        return resType(
            OperAuxParams<
                OpTags::SubstractByNum,
                typename resType::ItemType,
                typename resType::CategoryTag
            > (op2),
            std::forward<Op1>(op1)
        );
    } else {
        static_assert(DependencyFalse<Op1>);
    }

}

};