#include <iostream>

#include <vector>

#include <gtest/gtest.h>

#include "data/facilities/allocator.hpp"
#include "data/facilities/category.hpp"
#include "data/facilities/continuousMem.hpp"
#include "data/facilities/lowerAccess.hpp"
#include "data/facilities/traits.hpp"
#include "data/facilities/shape.hpp"

#include "data/tensor.hpp"
#include "data/trivialTensor.hpp"
#include "data/biasVector.hpp"
#include "data/zeroTensor.hpp"
#include "data/scalableTensor.hpp"
#include "data/dynamic.hpp"

#include "facilities/contMetaFuns/helper.hpp"
#include "facilities/contMetaFuns/sequential.hpp"

#include "operation/facilities/organizer.hpp"
#include "operation/facilities/operationFrame.hpp"
#include "operation/tensor/slice.hpp"
#include "operation/operator/sigmoid.hpp"

#include "operation/facilities/policies.hpp"

#include "policy/policy.hpp"

#include "policy/policyMacroBegin.hpp"

using namespace JYNN;



template <class Cont, 
          class=std::void_t< decltype(std::declval<Cont>().begin()) >,
          class=std::void_t< typename Cont::value_type >
          >
void Print(const Cont& cont) {
    std::copy(
        std::begin(cont),
        std::end(cont),
        std::ostream_iterator<typename Cont::value_type>(std::cout, " ")
    );
    std::cout << std::endl;
}

struct Placement {};


struct AA{
    using MajorClass = int;
    using MinorClass = char;
};

struct BB{
    using MajorClass = int;
    using MinorClass = char;
};


struct CC{
    using MajorClass = int;
    using MinorClass = char;
};


struct DD{
    using MajorClass = int;
    using MinorClass = char;
};

struct EE{
    using MajorClass = int;
    using MinorClass = char;
};


struct FF{
    using MajorClass = int;
    using MinorClass = char;
};


struct GG{
    using MajorClass = int;
    using MinorClass = std::string;
};
struct HH{
    using MajorClass = int;
    using MinorClass = int;
};

struct II{
    using MajorClass = int;
    using MinorClass = char;
};

struct JJ{
    using MajorClass = struct II;
    using MinorClass = struct HH;
};

struct KK{
    using MajorClass = struct HH;
    using MinorClass = struct JJ;
};

struct LL{
    using MajorClass = struct LL;
    using MinorClass = double;
};

struct MM{
    using MajorClass = struct HH;
    using MinorClass = struct JJ;
};

struct NN{
    using MajorClass = int;
    using MinorClass = std::string; 
};

ValuePolicyObj(PModif,            DimPolicy, ModifyDimNum, 3);
ValuePolicyObj(PDimCount,         DimPolicy, DimCount, 5);

TEST(operatioin, policy){
    using dimPolicy = PolicyContainer<PKeepDim, PModif, PDimCount>;
    using seltedPolicy = PolicySelect<DimPolicy, dimPolicy>;


    ASSERT_EQ(seltedPolicy::DimCount, 5);
    ASSERT_EQ(seltedPolicy::ModifyDimNum, 3);
    ASSERT_EQ(seltedPolicy::IsKeepDim, true);

    // ASSERT_EQ( (std::is_same_v<typename seltedPolicy::MinorClass, typename DimPolicy::IsKeepDimValueCate>), true);
};



TEST(operation, policy){

    Placement placement;

    using tensor0 = Tensor<0, int>;
    using Tensor4D = Tensor<4, long long>;
    using Tensor3DL = Tensor3D <long long>;
    using scalaTensor = ScalableTensor<Tensor3D<long long>>;
    Tensor4D  A(2, 7, 8, 10);
    Tensor4D  B(2, 7, 8, 10);
    Tensor3DF C(7, 8, 10);
    MatrixF   D(8, 10);
    ScalarF   E;  
    VectorF   F(10);
    Tensor3DL G(7, 8, 10);
    Tensor3DL H(7, 8, 10);
    scalaTensor sTensor;


//Policy Derive
    using DerivePolicyA = PolicyContainer<struct AA, struct BB, struct CC>;
    using DerivePolicyB = PolicyContainer<struct DD, struct EE, struct FF, struct GG, struct II, struct HH>;
    using DerivePolicy = PolicyDerive<DerivePolicyA, DerivePolicyB>;
    ASSERT_EQ(true, (std::is_same_v<DerivePolicy, PolicyContainer<struct AA, struct BB, struct CC, struct GG, struct HH>>) );


//Policy Select Picker Plain Derive Change Get
    using SubPolicyA = SubPolicyContainer<struct A, struct AA, struct BB, struct CC>;
    using SubPolicyB = SubPolicyContainer<struct B, Tensor3DF, Tensor3DF, std::string, float>;
    using SubPolicyC = SubPolicyContainer<struct A, struct DD, struct EE, struct FF, struct GG, struct II, struct HH>;
    using SubPolicyD = SubPolicyContainer<struct KK, struct LL>;

    using SubPolicyCont = PolicyContainer<SubPolicyA, SubPolicyB, SubPolicyC, struct JJ, SubPolicyD>;

    using PickSubPolicyA = SubPolicyPicker<SubPolicyCont, struct A>;
    ASSERT_EQ(true, (std::is_same_v<PickSubPolicyA, PolicyContainer<struct AA, struct BB, struct CC, struct DD, struct EE, struct FF, struct GG, struct II, struct HH, struct JJ>>));


    using ChangedPolicyA = ChangePolicy<struct MM, PickSubPolicyA>;
    ASSERT_EQ(true, (std::is_same_v<ChangedPolicyA, PolicyContainer<struct AA, struct BB, struct CC, struct DD, struct EE, struct FF, struct GG, struct II, struct HH, struct JJ, struct MM>>));

    using ChangedPolicyB = ChangePolicy<struct NN, PickSubPolicyA>;
    ASSERT_EQ(true, (std::is_same_v<ChangedPolicyB, PolicyContainer<struct AA, struct BB, struct CC, struct DD, struct EE, struct FF, struct II, struct HH, struct JJ, struct NN>>));

    using ChangedPolicyC = ChangePolicy<struct AA, PickSubPolicyA>;
    ASSERT_EQ(true, (std::is_same_v<ChangedPolicyC, PolicyContainer<struct GG, struct HH, struct JJ, struct AA>>));


    using GetedPolicyA = PolicyGet<PickSubPolicyA, struct II, struct HH>;
    ASSERT_EQ(true, (std::is_same_v<GetedPolicyA, struct JJ>));

    auto ExistPolicyA = PolicyExist<PickSubPolicyA, struct II, struct HH>;
    auto ExistPolicyB = PolicyExist<PickSubPolicyA, struct II, struct BAD>;
    ASSERT_EQ(true, ExistPolicyA);
    ASSERT_EQ(false, ExistPolicyB);
}




#include "policy/policyMacroEnd.hpp"