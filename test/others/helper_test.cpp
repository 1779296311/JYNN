
#include <gtest/gtest.h>

#include "facilities/contMetaFuns/valueSequential.hpp"
#include "operation/facilities/policies.hpp"

using namespace JYNN;
using namespace JYNN::Helper;

TEST(helper, order){
    

    using Indexs  = MakeIndexSequence<7>;
    auto x = ValueSequential::Order<IndexSequence<0,5,9,6,3,7,8,5>, 3>;
    ASSERT_EQ(x, 4);
};