#pragma once

namespace JYNN::OpTags {
    struct Sigmoid;
    struct SigMoidGrad;
};

namespace JYNN {

template <class Operand,
          class=std::void_t< std::enable_if<isValidOperand<OpTags::Sigmoid, Operand>> >
          >
auto Sigmoid(Operand& operand) {
    using rawOperand = RmConstRef<Operand>;
    using ResType = Operation<OpTags::Sigmoid, OperandContainer<rawOperand>>;
    return ResType(std::forward<Operand>(operand));
}

};