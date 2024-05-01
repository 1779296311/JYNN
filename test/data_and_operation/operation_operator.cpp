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

#include "operation/tensor/permute.hpp"
#include "operation/operator/reduceSum.hpp"
#include "operation/operator/abs.hpp"
#include "operation/operator/acos.hpp"
#include "operation/operator/acosh.hpp"
#include "operation/operator/asin.hpp"
#include "operation/operator/asinh.hpp"
#include "operation/operator/sin.hpp"
#include "operation/operator/cos.hpp"
#include "operation/operator/tan.hpp"
#include "operation/operator/tanh.hpp"
#include "operation/operator/exp.hpp"
#include "operation/operator/negative.hpp"
#include "operation/operator/sign.hpp"
#include "operation/operator/square.hpp"
#include "operation/operator/substract.hpp"
#include "operation/operator/multiply.hpp"
#include "operation/operator/divide.hpp"
#include "operation/operator/negative.hpp"
#include "operation/tensor/reshape.hpp"
#include "operation/tensor/tile.hpp"

struct Placement {};

using namespace JYNN;

using tensor0 = Tensor<0, int>;
using Tensor4D = Tensor<4, long long>;
using Tensor4DF = Tensor<4, float>;
using Tensor3DL = Tensor3D <long long>;
using scalaTensor = ScalableTensor<Tensor3D<long long>>;

ValuePolicyObj(PModif,            DimPolicy, ModifyDimNum, 3);
ValuePolicyObj(PDimCount,         DimPolicy, DimCount, 5);


// 如果 有 用一个值 操作矩阵 那 这个值的类型 会被转换为 矩阵中值的类型；

TEST(operator, tile){
    Tensor4DF  A(2, 7, 8, 2);
    auto x = Tile<PolicyContainer<PDimArrayIs<0,1>>>(A, Shape(2, 2, 4, 7, 8, 2));
}

TEST(operator, reshape) {
    Tensor4DF  A(2, 7, 8, 10);
    auto x = Reshape(A, Shape(14, 8, 10));
    auto y = Reshape(x, Shape(2, 28, 2, 2, 5));
    auto z = Reshape(x, Shape(InferredDim, 28, 20));
    auto& shape = x.GetShape();

    ASSERT_EQ(shape.dimSize_, 3);
    ASSERT_EQ(shape.Count(), 14 * 8 * 10);
    ASSERT_EQ(shape[0], 14);
    ASSERT_EQ(shape[1], 8);    
    ASSERT_EQ(shape[2], 10);

    auto& shapey = y.GetShape();
    ASSERT_EQ(shapey.dimSize_, 5);
    ASSERT_EQ(shapey.Count(), 2 * 28 * 2 * 2 * 5);
    ASSERT_EQ(shapey[0], 2);
    ASSERT_EQ(shapey[1], 28);
    ASSERT_EQ(shapey[2], 2);    
    ASSERT_EQ(shapey[3], 2);
    ASSERT_EQ(shapey[4], 5);

    auto& shapez = z.GetShape();
    ASSERT_EQ(shapez.dimSize_, 3);
    ASSERT_EQ(shapez.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shapez[0], 2);
    ASSERT_EQ(shapez[1], 28);
    ASSERT_EQ(shapez[2], 20);    
}

TEST(operator, divide) {
    Tensor4DF  A(2, 7, 8, 10);
    Tensor4DF  B(2, 7, 8, 10);
    float c = 3.99;
    auto x =  A / B;
    auto y = c / x;
    auto z = y / c;

    auto& shape = x.GetShape();
    ASSERT_EQ(shape.dimSize_, 4);
    ASSERT_EQ(shape.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shape[0], 2);
    ASSERT_EQ(shape[1], 7);
    ASSERT_EQ(shape[2], 8);    
    ASSERT_EQ(shape[3], 10);

    auto& shapey = y.GetShape();
    ASSERT_EQ(shapey.dimSize_, 4);
    ASSERT_EQ(shapey.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shapey[0], 2);
    ASSERT_EQ(shapey[1], 7);
    ASSERT_EQ(shapey[2], 8);    
    ASSERT_EQ(shapey[3], 10);

    auto& shapez = z.GetShape();
    ASSERT_EQ(shapez.dimSize_, 4);
    ASSERT_EQ(shapez.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shapez[0], 2);
    ASSERT_EQ(shapez[1], 7);
    ASSERT_EQ(shapez[2], 8);    
    ASSERT_EQ(shapez[3], 10);;

    ASSERT_EQ(y.AuxParams().Value(), c);
    ASSERT_EQ(z.AuxParams().Value(), c);
}

TEST(operator, multiply) {
    Tensor4DF  A(2, 7, 8, 10);
    Tensor4DF  B(2, 7, 8, 10);
    float c = 3.99;
    auto x =  A * B;
    auto y = c * x;
    auto z = y * c;

    auto& shape = x.GetShape();
    ASSERT_EQ(shape.dimSize_, 4);
    ASSERT_EQ(shape.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shape[0], 2);
    ASSERT_EQ(shape[1], 7);
    ASSERT_EQ(shape[2], 8);    
    ASSERT_EQ(shape[3], 10);

    auto& shapey = y.GetShape();
    ASSERT_EQ(shapey.dimSize_, 4);
    ASSERT_EQ(shapey.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shapey[0], 2);
    ASSERT_EQ(shapey[1], 7);
    ASSERT_EQ(shapey[2], 8);    
    ASSERT_EQ(shapey[3], 10);

    auto& shapez = z.GetShape();
    ASSERT_EQ(shapez.dimSize_, 4);
    ASSERT_EQ(shapez.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shapez[0], 2);
    ASSERT_EQ(shapez[1], 7);
    ASSERT_EQ(shapez[2], 8);    
    ASSERT_EQ(shapez[3], 10);;

    ASSERT_EQ(y.AuxParams().Value(), c);
    ASSERT_EQ(z.AuxParams().Value(), c);
}


TEST(operator, substract) {
    Tensor4DF  A(2, 7, 8, 10);
    float a = 3.1415926;
    auto x = A - a;
    auto y = a - x;
    auto z = x + y;
    auto& shape = x.GetShape();
    ASSERT_EQ(shape.dimSize_, 4);
    ASSERT_EQ(shape.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shape[0], 2);
    ASSERT_EQ(shape[1], 7);
    ASSERT_EQ(shape[2], 8);    
    ASSERT_EQ(shape[3], 10);

    auto& shapey = y.GetShape();
    ASSERT_EQ(shapey.dimSize_, 4);
    ASSERT_EQ(shapey.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shapey[0], 2);
    ASSERT_EQ(shapey[1], 7);
    ASSERT_EQ(shapey[2], 8);    
    ASSERT_EQ(shapey[3], 10);

    auto& shapez = z.GetShape();
    ASSERT_EQ(shapez.dimSize_, 4);
    ASSERT_EQ(shapez.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shapez[0], 2);
    ASSERT_EQ(shapez[1], 7);
    ASSERT_EQ(shapez[2], 8);    
    ASSERT_EQ(shapez[3], 10);  

    ASSERT_EQ(x.AuxParams().Value(), a);
    ASSERT_EQ(y.AuxParams().Value(), a);
}

TEST(operator, signalop) {
    Tensor4D  A(2, 7, 8, 10);
    auto x = Abs(A);
    auto z = Exp(A);
    auto y = -A;

    const auto& shape = x.GetShape();
    ASSERT_EQ(shape.dimSize_, 4);
    ASSERT_EQ(shape.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shape[0], 2);
    ASSERT_EQ(shape[1], 7);
    ASSERT_EQ(shape[2], 8);    
    ASSERT_EQ(shape[3], 10);

    auto& shapey = y.GetShape();
    ASSERT_EQ(shapey.dimSize_, 4);
    ASSERT_EQ(shapey.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shapey[0], 2);
    ASSERT_EQ(shapey[1], 7);
    ASSERT_EQ(shapey[2], 8);    
    ASSERT_EQ(shapey[3], 10);

    auto& shapez = z.GetShape();
    ASSERT_EQ(shapez.dimSize_, 4);
    ASSERT_EQ(shapez.Count(), 2 * 7 * 8 * 10);
    ASSERT_EQ(shapez[0], 2);
    ASSERT_EQ(shapez[1], 7);
    ASSERT_EQ(shapez[2], 8);    
    ASSERT_EQ(shapez[3], 10); 

}


TEST(operator, reduceSum) {
    Tensor4DF  A(2, 7, 8, 10);
    auto x = ReduceSum<PolicyContainer<PDimArrayIs<1>>>(A);

    const auto& shape = x.GetShape();
    ASSERT_EQ(shape.dimSize_, 3);
    ASSERT_EQ(shape.Count(), 2 * 8 * 10);
    ASSERT_EQ(shape[0], 2);
    ASSERT_EQ(shape[1], 8);
    ASSERT_EQ(shape[2], 10);

}

TEST(operator, permute) {
    Tensor4DF  A(2, 7, 8, 10);
    Tensor4DF  B(8, 10, 3, 5);

    auto x = Permute<PolicyContainer<PDimArrayIs<2, 0, 1, 3>>>(A);
    auto& shape = x.GetShape();
    ASSERT_EQ(shape[0], 8);
    ASSERT_EQ(shape[1], 2);
    ASSERT_EQ(shape[2], 7);
    ASSERT_EQ(shape[3], 10);
    auto y = PermuteInv<PolicyContainer<PDimArrayIs<2, 0, 1, 3>>>(x);
    auto& shape1 = y.GetShape();
    ASSERT_EQ(shape1[0], 2);
    ASSERT_EQ(shape1[1], 7);
    ASSERT_EQ(shape1[2], 8);
    ASSERT_EQ(shape1[3], 10);

}


TEST(operator, Dot) {

    Tensor4DF  A(2, 7, 8, 10);
    Tensor4DF  B(8, 10, 3, 5);
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
    Tensor4DF  A(2, 7, 8, 10);
    auto x = Sigmoid(A);
    const auto& xshape = x.GetShape();
ASSERT_EQ(xshape.Count(), 2 * 7 * 8 * 10);    
}

TEST(operator, operatorAddWithNumber) {
    Tensor3DF C(7, 8, 10);
    float test = 3.1415926;
    auto x = C + test;
    auto y = test + C;
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