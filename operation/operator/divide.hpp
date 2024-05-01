#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"


namespace JYNN::OpTags{
    struct Divide;
    struct DivideFromNum;
    struct DivideByNum;
};


namespace JYNN{

namespace DivideFromNumInner{

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
constexpr bool isValidOperand<OpTags::DivideFromNum, Number, Operand> {
    DivideFromNumInner::Valid<Number, Operand>()
};


template <class Item, class Cate>
struct OperAuxParams<OpTags::DivideFromNum, Item, Cate> : public OperAuxValue<Item> {
    using Base = OperAuxValue<Item>;
    using Base::Base;
    using Base::operator=;
};


namespace DivideByNumInner{

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
constexpr bool isValidOperand<OpTags::DivideByNum,  Operand, Number> {
    DivideByNumInner::Valid<Operand, Number>()
};

template <class Item, class Cate>
struct OperAuxParams<OpTags::DivideByNum, Item, Cate> : public OperAuxValue<Item> {
    using Base = OperAuxValue<Item>;
    using Base::Base;
    using Base::operator=;
};

template <class Op1, class Op2,
          class = std::enable_if_t<
            isValidOperand<OpTags::Divide, Op1, Op2> ||
            isValidOperand<OpTags::DivideFromNum, Op1, Op2> ||
            isValidOperand<OpTags::DivideByNum, Op1, Op2>
          >
         >
auto operator/ (Op1&& op1, Op2&& op2) {

    if constexpr (isValidOperand<OpTags::Divide, Op1, Op2>) {
        using rawOp1 = RmConstRef<Op1>;
        using rawOp2 = RmConstRef<Op2>;
        using resType = Operation<
            OpTags::Divide,
            OperandContainer<rawOp1, rawOp2>
        >;
        return resType(std::forward<Op1>(op1), std::forward<Op2>(op2));

    } else if constexpr (isValidOperand<OpTags::DivideFromNum, Op1, Op2>) {

        using rawOp2 = RmConstRef<Op2>;
        using resType = Operation<
            OpTags::DivideFromNum,
            OperandContainer<rawOp2>
        >;
        return resType(
            OperAuxParams<
                OpTags::DivideFromNum,
                typename resType::ItemType,
                typename resType::CategoryTag
            > (op1),
            std::forward<Op2>(op2)
        );

    } else if constexpr (isValidOperand<OpTags::DivideByNum, Op1, Op2>) {
        using rawOp1 = RmConstRef<Op1>;
        using resType = Operation<
            OpTags::DivideByNum,
            OperandContainer<rawOp1>
        >;
        return resType(
            OperAuxParams<
                OpTags::DivideByNum,
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