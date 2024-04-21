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

#include "policy/policy.hpp"

#include "operation/operator/sigmoid.hpp"
#include "operation/operator/add.hpp"

#include "operation/tensor/dot.hpp"
#include "policy/policyMacroBegin.hpp"
struct Placement {};

using namespace JYNN;

using tensor0 = Tensor<0, int>;
using Tensor4D = Tensor<4, long long>;
using Tensor3DL = Tensor3D <long long>;
using scalaTensor = ScalableTensor<Tensor3D<long long>>;

ValuePolicyObj(PModif,            DimPolicy, ModifyDimNum, 3);
ValuePolicyObj(PDimCount,         DimPolicy, DimCount, 5);

TEST(operator, Dot) {

    Tensor4D  A(2, 7, 8, 10);
    Tensor4D  B(8, 10, 3, 5);
    using dimPolicy = PolicyContainer<PKeepDim, PModif, PDimCount>;
    using DotDeduceTest = OperCategoryDeduce<OpTags::Dot, dimPolicy, Tensor4D, Tensor4D>;
ASSERT_EQ(DotDeduceTest::dim_, 4 + 4 - 2 * 3);

    auto DotAB = Dot<PolicyContainer<PModifyDimNumIs<2>>>(A, B);

    auto DABShape = DotAB.GetShape();
ASSERT_EQ(DABShape.Count(), 2 * 7 * 3 * 5);


    Tensor <5, double> tensor5D (1, 2, 4, 5, 1);
    Tensor <6, float> tensor6D (4, 5, 1, 2, 3, 4);
    auto Dot56 = Dot<PolicyContainer<PModifyDimNumIs<3>>>(tensor5D, tensor6D);
    auto DotTestShape = Dot56.GetShape();
ASSERT_EQ(DotTestShape.Count(), 1*2*2*3*4);
}   



TEST(operator, operatorSigmoid) {
    Tensor4D  A(2, 7, 8, 10);
    auto x = Sigmoid(A);
    const auto& xshape = x.GetShape();
ASSERT_EQ(xshape.Count(), 2 * 7 * 8 * 10);    
}



TEST(operator, operatorAddWithNumber) {
    Tensor3DF C(7, 8, 10);
    float test = 3.1415926;
    auto x = C + test;
    auto param = x.AuxParams().Value();
ASSERT_EQ(param - test, 0);


    auto shape = x.GetShape();
ASSERT_EQ(shape.Count(), 7 * 8 * 10);


}

TEST(operation, operatorAdd) {
    Placement placement;

    Tensor4D  A(2, 7, 8, 10);
    Tensor4D  B(2, 7, 8, 10);
    Tensor3DF C(7, 8, 10);
    MatrixF   D(8, 10);
    ScalarF   E;  
    VectorF   F(10);
    Tensor3DL G(7, 8, 10);
    Tensor3DL H(7, 8, 10);
    scalaTensor sTensor;

    for(int i=0; i<7; ++i) {
        for(int j=0; j<8; ++j) {
            for(int k=0; k<10; ++k) {
                A.SetValue(0, i, j, k, i*100 + j*10 + k);
                B.SetValue(1, i, j, k, k*100 + j*10 + i);
            }
        }
    }

auto testadd = A + B;

    for(int i=0; i<7; ++i) {
        for(int j=0; j<8; ++j) {
            for(int k=0; k<10; ++k) {
                ASSERT_EQ(A[0][i][j][k].Value(), i*100 + j*10 + k);
                ASSERT_EQ(B[1][i][j][k].Value(), k*100 + j*10 + i);
            }
        }
    }


auto testO = Operation<OpTags::Add, OperandContainer<Tensor4D, Tensor4D>>(A, B);
const auto& testOp1 = testO.Operand<0>();
const auto& testOp2 = testO.Operand<1>();



    for(int i=0; i<7; ++i) {
        for(int j=0; j<8; ++j) {
            for(int k=0; k<10; ++k) {
                ASSERT_EQ(testOp1[0][i][j][k].Value(), i*100 + j*10 + k);
                ASSERT_EQ(testOp2[1][i][j][k].Value(), k*100 + j*10 + i);
            }
        }
    }
// add tensor and tensor
    auto addx = A + B;
    auto addxShape = addx.GetShape();
ASSERT_EQ(addxShape.Count(), 2 * 7 * 8 * 10);

    const auto& addxOp1 = addx.Operand<0>();
    const auto& addxOp2 = addx.Operand<1>();
ASSERT_EQ(addxOp1.GetShape().Count(), 2*7*8*10);
ASSERT_EQ(addxOp2.GetShape().Count(), 2*7*8*10);
ASSERT_EQ(true, (std::is_same_v<RmConstRef<decltype(addxOp1)>, Tensor4D>) );
ASSERT_EQ(true, (std::is_same_v<RmConstRef<decltype(addxOp2)>, Tensor4D>) );

    for(int i=0; i<7; ++i) {
        for(int j=0; j<8; ++j) {
            for(int k=0; k<10; ++k) {
               ASSERT_EQ(addxOp1[0][i][j][k].Value(), i*100 + j*10 + k);
               ASSERT_EQ(addxOp2[1][i][j][k].Value(), k*100 + j*10 + i);
            }
        }
    }

}

#include "policy/policyMacroEnd.hpp"