#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"


namespace JYNN::OpTags {
    struct Cos;
    struct CosGrad;
}


namespace JYNN{


template <class Operand,
          class = std::enable_if<isValidOperand<OpTags::Cos, Operand>> >
auto Cos(Operand&& op) {
    using rawOp = RmConstRef<Operand>;
    using resType = Operation<OpTags::Cos, OperandContainer<rawOp>>;
    return resType(std::forward<Operand>(op));
}


template <class Grad, class Input,
          class = std::enable_if<isValidOperand<OpTags::CosGrad, Grad, Input>> >
auto CosGrad(Grad&& p_grad, Input&& p_input) {
    
    static_assert(DataCategory<Input>::dim_ >= DataCategory<Grad>::dim_);
    using rawGrad = RmConstRef<Grad>;
    using rawInput = RmConstRef<Input>;
    using ResType = Operation<OpTags::CosGrad, OperandContainer<rawGrad, rawInput>>;
    return ResType(std::forward<Grad>(p_grad), std::forward<Input>(p_input));
}
}