#include <gtest/gtest.h>

#include "data/facilities/allocator.hpp"
#include "data/facilities/category.hpp"
#include "data/facilities/continuousMem.hpp"
#include "data/facilities/lowerAccess.hpp"
#include "data/facilities/traits.hpp"
#include "data/facilities/shape.hpp"
#include "facilities/contMetaFuns/helper.hpp"
#include "data/tensor.hpp"
#include "data/trivialTensor.hpp"
#include "data/biasVector.hpp"
#include "data/zeroTensor.hpp"
#include "data/scalableTensor.hpp"
#include "data/dynamic.hpp"
#include "operation/facilities/organizer.hpp"
#include "facilities/contMetaFuns/sequential.hpp"
#include "operation/facilities/operationFrame.hpp"
#include "operation/tensor/slice.hpp"

using namespace JYNN;
struct Placement{};

TEST(test_data, data){
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
    for(int i=0; i<7; ++i) {
        for(int j=0; j<8; ++j) {
            for(int k=0; k<10; ++k) {
                G.SetValue(i, j, k, i*100 + j*10 + k);
                H.SetValue(i, j, k, k*100 + j*10 + i);
            }
        }
    }

    using commOperCon = OperandContainer<Tensor4D, Tensor3DL, Tensor4D, Tensor3DF, MatrixF, ScalarF, VectorF, Tensor3DL>;
    using commOperation = Operation<Placement, commOperCon>;
    commOperation commOp(A, G, B, C, D, E, F, H);
    auto commShape = commOp.GetShape();

    ASSERT_EQ(commShape.dimSize_, 4);
    ASSERT_EQ(commShape.Count(), 2 * 7 * 8 * 10);


    auto op1 = commOp.Operand<1>();
    auto op2 = commOp.Operand<7>();

    for(int i=0; i<7; ++i) {
        for(int j=0; j<8; ++j) {
            for(int k=0; k<10; ++k) {
                ASSERT_EQ(op1[i][j][k].Value(), i*100 + j*10 + k);
                ASSERT_EQ(op2[i][j][k].Value(), k*100 + j*10 + i);
            }
        }
    }
  
}

TEST(test_operationFrame, operationFrame) {
    Placement placement;
    using Tensor4D = Tensor<4, long long>;
    Tensor4D  A(2, 7, 8, 10);

//Slice
    using commOperCon = OperandContainer<Tensor4D>;
    using commOperation = Operation<Placement, commOperCon>;
    commOperation commOp(A);
    auto x = commOp[0];
    auto shapeA = A.GetShape();
    auto shapex = x.GetShape();
    ASSERT_EQ(shapeA.dimSize_, shapex.dimSize_ + 1);
    ASSERT_EQ(shapeA.Count(), shapex.Count() * 2);

}