#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"


namespace JYNN::OpTags{
    struct Asin;
    struct AsinGrad;
};

namespace JYNN{


template <class Operand,
          class = std::enable_if<isValidOperand<OpTags::Asin, Operand>> >
auto Asin(Operand&& op) {
    using rawOp = RmConstRef<Operand>;
    using resType = Operation<OpTags::Asin, OperandContainer<rawOp>>;
    return resType(std::forward<Operand>(op));
}


template <class Grad, class Input,
          class = std::enable_if<isValidOperand<OpTags::AsinGrad, Grad, Input>> >
auto AsinGrad(Grad&& grad, Input&& input) {
    
    static_assert(DataCategory<Input>::dim_ >= DataCategory<Grad>::dim_);
    using rawGrad = RmConstRef<Grad>;
    using rawInput = RmConstRef<Input>;
    using resType = Operation<OpTags::AsinGrad, OperandContainer<rawGrad, rawInput>>;
    return resType(std::forward<Grad>(grad), std::forward<Input>(input)) ;
}

};