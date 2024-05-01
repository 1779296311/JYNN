#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"


namespace JYNN::OpTags {
    struct Asinh;
    struct AsinhGrad;
}


namespace JYNN{


template <class Operand,
          class = std::enable_if<isValidOperand<OpTags::Asinh, Operand>> >
auto Asinh(Operand&& op) {
    using rawOp = RmConstRef<Operand>;
    using resType = Operation<OpTags::Asinh, OperandContainer<rawOp>>;
    return resType(std::forward<Operand>(op));
}


template <class Grad, class Input,
          class = std::enable_if<isValidOperand<OpTags::AsinhGrad, Grad, Input>> >
auto AsinhGrad(Grad&& p_grad, Input&& p_input) {
    
    static_assert(DataCategory<Input>::dim_ >= DataCategory<Grad>::dim_);
    
    using rawGrad = RmConstRef<Grad>;
    using rawInput = RmConstRef<Input>;
    using ResType = Operation<OpTags::AsinhGrad, OperandContainer<rawGrad, rawInput>>;
    return ResType(std::forward<Grad>(p_grad), std::forward<Input>(p_input));
}
}