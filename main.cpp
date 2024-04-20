#include <iostream>

#include <vector>

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


#include "policy/policy.hpp"
// struct A{
//     using CategoryTag = JYNN::Category::Dim<4>;
// };

// struct B{
//     using CategoryTag = JYNN::Category::Dim<2>;
// };

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

int main () {

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

//Sigmoid
    auto x = Sigmoid(A);
    const auto& xshape = x.GetShape();
    std::cout << xshape.Count() << std::endl;  



//Policy Derive
    using DerivePolicyA = PolicyContainer<struct AA, struct BB, struct CC>;
    using DerivePolicyB = PolicyContainer<struct DD, struct EE, struct FF, struct GG, struct II, struct HH>;
    using DerivePolicy = PolicyDerive<DerivePolicyA, DerivePolicyB>;
std::cout << (std::is_same_v<DerivePolicy, PolicyContainer<struct AA, struct BB, struct CC, struct GG, struct HH>>) << std::endl;


//Policy Select Picker Plain Derive Change Get
    using SubPolicyA = SubPolicyContainer<struct A, struct AA, struct BB, struct CC>;
    using SubPolicyB = SubPolicyContainer<struct B, Tensor3DF, Tensor3DF, std::string, float>;
    using SubPolicyC = SubPolicyContainer<struct A, struct DD, struct EE, struct FF, struct GG, struct II, struct HH>;
    using SubPolicyD = SubPolicyContainer<struct KK, struct LL>;

    using SubPolicyCont = PolicyContainer<SubPolicyA, SubPolicyB, SubPolicyC, struct JJ, SubPolicyD>;

    using PickSubPolicyA = SubPolicyPicker<SubPolicyCont, struct A>;
std::cout << (std::is_same_v<PickSubPolicyA, PolicyContainer<struct AA, struct BB, struct CC, struct DD, struct EE, struct FF, struct GG, struct II, struct HH, struct JJ>>) << std::endl;

    using ChangedPolicyA = ChangePolicy<struct MM, PickSubPolicyA>;
std::cout << (std::is_same_v<ChangedPolicyA, PolicyContainer<struct AA, struct BB, struct CC, struct DD, struct EE, struct FF, struct GG, struct II, struct HH, struct JJ, struct MM>>) << std::endl;

    using ChangedPolicyB = ChangePolicy<struct NN, PickSubPolicyA>;
std::cout << (std::is_same_v<ChangedPolicyB, PolicyContainer<struct AA, struct BB, struct CC, struct DD, struct EE, struct FF, struct II, struct HH, struct JJ, struct NN>>) << std::endl;

    using ChangedPolicyC = ChangePolicy<struct AA, PickSubPolicyA>;
std::cout << (std::is_same_v<ChangedPolicyC, PolicyContainer<struct GG, struct HH, struct JJ, struct AA>>) << std::endl;

    using GetedPolicyA = PolicyGet<PickSubPolicyA, struct II, struct HH>;
std::cout << (std::is_same_v<GetedPolicyA, struct JJ>) << std::endl;

    auto ExistPolicyA = PolicyExist<PickSubPolicyA, struct II, struct HH>;
    auto ExistPolicyB = PolicyExist<PickSubPolicyA, struct II, struct BAD>;
std::cout << ExistPolicyA << "  " << ExistPolicyB << std::endl;

    // for(int i=0; i<7; ++i) {
    //     for(int j=0; j<8; ++j) {
    //         for(int k=0; k<10; ++k) {
    //             G.SetValue(i, j, k, i*100 + j*10 + k);
    //             H.SetValue(i, j, k, k*100 + j*10 + i);
    //         }
    //     }
    // }
//Slice

    // using commOperCon = OperandContainer<Tensor4D>;
    // using commOperation = Operation<Placement, commOperCon>;
    // commOperation commOp(A);
    // auto x = commOp[0];
    // auto shapeA = A.GetShape();
    // auto shapex = x.GetShape();
    // std::cout << shapeA.dimSize_ << "   " << shapeA.Count() << std::endl;
    // std::cout << shapex.dimSize_ << "   " << shapex.Count() << std::endl;



    // OperShapeInfo<int, int, DataCategory<Tensor4D>> shapeInfo(placement, F, D, A, E, B, A);
    // auto commonShape = shapeInfo.GetShape();
    // std::cout << commonShape.dimSize_ << "   " << commonShape.Count() << std::endl;

    // using commOperCon = OperandContainer<Tensor4D, Tensor3DL, Tensor4D, Tensor3DF, MatrixF, ScalarF, VectorF, Tensor3DL>;
    // using commOperation = Operation<Placement, commOperCon>;
    // commOperation commOp(A, G, B, C, D, E, F, H);
    // auto commShape = commOp.GetShape();
    // std::cout << commShape.dimSize_ << "   " << commShape.Count() << std::endl;

    // auto op1 = commOp.Operand<1>();
    // auto op2 = commOp.Operand<7>();

    // for(int i=0; i<7; ++i) {
    //     for(int j=0; j<8; ++j) {
    //         for(int k=0; k<10; ++k) {
    //             std::cout << op1[i][j][k].Value() << "  " 
    //                       << op2[i][j][k].Value() << std::endl;
    //         }
    //     }
    // }



    // using commomCate = 
    //    OperCategoryDeduce<int, int, tensor0, Tensor3DF, VectorF, ScalarF, Tensor4D>;
    // std::cout << commomCate::dim_ << std::endl;



    // auto dynaminData = MakeDynamic(B);
    // auto x = dynaminData.Value().Value();
    
    // sTensor.PushBack(A);
    // sTensor.PushBack(x);
    // sTensor.PushBack(x);

    // for(int i=0; i<6; ++i) {
    //     for(int j=0; j<7; ++j) {
    //         for(int k=0; k<8; ++k) {
    //             std::cout << sTensor[0][i][j][k].Value() << "  " 
    //                       << sTensor[1][i][j][k].Value() << std::endl;
    //         }
    //     }
    // }

    // auto x = sTensor[0][1][2][4];
    // auto x1 = sTensor[1][4][6][5];
    // auto x2 = sTensor[0][0][1][3];

    // std::cout << A(1, 2, 4) << std::endl;
    // std::cout << A(0, 1, 3) << std::endl;   
    // std::cout << B(4, 6, 5) << std::endl;

    // std::cout << x.Value() << std::endl;
    // std::cout << x1.Value() << std::endl;
    // std::cout << x2.Value() << std::endl;

    // auto a = A(1, 2, 4);
    // auto c = A[1][2][4];
    // auto d = A[0][1][3];
    // std::cout << c.Value() << " " << d.Value() << std::endl;
    // TrivialTensor x(d, 1, 2);
    // std::cout << x.Value().Value() << std::endl;
    // BiasVector bias(5, d, 3);
    // std::cout << bias.Value().Value() << std::endl;

    // Shape<3> s1, s2;
    // std::copy(
    //     std::begin(s1) + 1,
    //     std::end(s1),
    //     std::begin(s2)
    // );

    // std::cout << a << std::endl;
    // std::cout << std::is_same_v<tensor0::DeviceType,  Device::CPU> << std::endl;
    // Shape A(2,3,5);
    // std::cout << A.Count() << std::endl;
    // std::cout << A.IndexToOffset(1, 2, 4) << std::endl;
    // auto a = A.OffsetToIndex(A.IndexToOffset(1, 2, 3));
    // Print(a);
    // A.ShiftIndex(a);
    // Print(a);

    //using a = Helper::MakeIndexSequence<21>;
    // A a;
    // B b;
    // std::cout << (b == a) << std::endl;
    // std::array<size_t, 3> darr {2, 2, 3};
    // size_t gap = 1;
    // std::cout << ShapeInner::IndexToShift(darr, gap, 1, 1, 1) << std::endl;;
    // std::vector<int> X;
    // auto x = X.begin();
    //  auto x = JYNN::isIterator<decltype(3)>;
    //  std::cout << x << std::endl;
    // std::cout << "This is a test!" << std::endl;
    // JYNN::Allocator<JYNN::Device::CPU>::Allocate<char>(1025);

    // using a = JYNN::PrincipalDataType<int, JYNN::Device::CPU, JYNN::Category::Dim<0>>;
    // std::cout << JYNN::isValidCategory<JYNN::Category::Dim<0>> << std::endl;
    // using c = JYNN::DataCategory<int>;
    // using b = JYNN::DataCategory<A>;
    // auto x = JYNN::is3DArray<A>;
    // std::cout << x << std::endl;
    return 0;
}