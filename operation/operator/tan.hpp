#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"


namespace JYNN::OpTags {
    struct Tan;
    struct TanGrad;
}


namespace JYNN{


template <class Operand,
          class = std::enable_if<isValidOperand<OpTags::Tan, Operand>> >
auto Tan(Operand&& op) {
    using rawOp = RmConstRef<Operand>;
    using resType = Operation<OpTags::Tan, OperandContainer<rawOp>>;
    return resType(std::forward<Operand>(op));
}


template <class Grad, class Input,
          class = std::enable_if<isValidOperand<OpTags::TanGrad, Grad, Input>> >
auto TanGrad(Grad&& p_grad, Input&& p_input) {
    
    static_assert(DataCategory<Input>::dim_ >= DataCategory<Grad>::dim_);
    using rawGrad = RmConstRef<Grad>;
    using rawInput = RmConstRef<Input>;
    using ResType = Operation<OpTags::TanGrad, OperandContainer<rawGrad, rawInput>>;
    return ResType(std::forward<Grad>(p_grad), std::forward<Input>(p_input));
}
}