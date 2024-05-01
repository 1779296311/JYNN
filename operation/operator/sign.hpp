#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"

namespace JYNN::OpTags {
    struct Sign;
}

namespace JYNN {

template <typename TP,
          class = std::enable_if<isValidOperand<OpTags::Sign, TP>> >
auto Sign(TP&& p_m)
{
    using rawM = RmConstRef<TP>;
    using ResType = Operation<OpTags::Sign, OperandContainer<rawM>>;
    return ResType(std::forward<TP>(p_m));

}
}