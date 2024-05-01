#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"


namespace JYNN::OpTags{
    struct Acos;
    struct AcosGrad;
};

namespace JYNN{


template <class Operand,
          class = std::enable_if<isValidOperand<OpTags::Acos, Operand>> >
auto Acos(Operand&& op) {
    using rawOp = RmConstRef<Operand>;
    using resType = Operation<OpTags::Acos, OperandContainer<rawOp>>;
    return resType(std::forward<Operand>(op));
}


template <class Grad, class Input,
          class = std::enable_if<isValidOperand<OpTags::AcosGrad, Grad, Input>> >
auto AcosGrad(Grad&& grad, Input&& input) {

    static_assert(DataCategory<Input>::dim_ >= DataCategory<Grad>::dim_);

    using rawGrad = RmConstRef<Grad>;
    using rawInput = RmConstRef<Input>;
    using resType = Operation<OpTags::AcosGrad, OperandContainer<rawGrad, rawInput>>;
    return resType(std::forward<Grad>(grad), std::forward<Input>(input)) ;
}

};