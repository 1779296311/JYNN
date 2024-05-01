#pragma once

#include "data/facilities/shape.hpp"
#include "data/facilities/traits.hpp"
#include "operation/facilities/policies.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/valueSequential.hpp"
#include "facilities/traits.hpp"


namespace JYNN::OpTags{
    struct Negative;
}

namespace JYNN {


template <class TP,
          class = std::enable_if<isValidOperand<OpTags::Negative, TP>> >
auto operator- (TP&& p_m) {
    using rawM = RmConstRef<TP>;
    using ResType = Operation<OpTags::Negative, OperandContainer<rawM>>;
    return ResType(std::forward<TP>(p_m));
}
}