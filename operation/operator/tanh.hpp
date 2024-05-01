#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"


namespace JYNN::OpTags {
    struct Tanh;
    struct TanhGrad;
}


namespace JYNN{


template <class Operand,
          class = std::enable_if<isValidOperand<OpTags::Tanh, Operand>> >
auto Tanh(Operand&& op) {
    using rawOp = RmConstRef<Operand>;
    using resType = Operation<OpTags::Tanh, OperandContainer<rawOp>>;
    return resType(std::forward<Operand>(op));
}


template <class Grad, class Input,
          class = std::enable_if<isValidOperand<OpTags::TanhGrad, Grad, Input>> >
auto TanhGrad(Grad&& p_grad, Input&& p_input) {
    
    static_assert(DataCategory<Input>::dim_ >= DataCategory<Grad>::dim_);
    using rawGrad = RmConstRef<Grad>;
    using rawInput = RmConstRef<Input>;
    using ResType = Operation<OpTags::TanhGrad, OperandContainer<rawGrad, rawInput>>;
    return ResType(std::forward<Grad>(p_grad), std::forward<Input>(p_input));
}
}