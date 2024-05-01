#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"


namespace JYNN::OpTags{
    struct Abs;
};

namespace JYNN{


template <class Operand,
          class = std::enable_if<isValidOperand<OpTags::Abs, Operand>> >
auto Abs(Operand&& op) {
    using rawOp = RmConstRef<Operand>;
    using resType = Operation<OpTags::Abs, OperandContainer<rawOp>>;
    return resType(std::forward<Operand>(op));
}

};