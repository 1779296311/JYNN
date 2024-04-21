#include "data/facilities/category.hpp"
#include "facilities/traits.hpp"
#include "data/tensor.hpp"
#include "operation/facilities/organizer.hpp"

#pragma once

namespace JYNN::OpTags{
    struct Add;
    struct AddWithNumber;
};

namespace JYNN {


/*************************** isValidOperand add with number **************************************************/
namespace AddwithNumberInner{

template <class Operand1, class Operand2>
constexpr bool Valid() {
    if constexpr ( isValidCategory<DataCategory<Operand1>> &&
                  !isValidCategory<DataCategory<Operand2>>){
        return std::is_constructible_v<
            typename RmConstRef<Operand1>::ItemType,
            Operand2
        >;
    } else if constexpr ( !isValidCategory<DataCategory<Operand1>> &&
                           isValidCategory<DataCategory<Operand2>>){
        return std::is_constructible_v<
            typename RmConstRef<Operand2>::ItemType,
            Operand1
        >;
    } else {
        return false;
    }
}

};

template <class Operand1, class Operand2>
constexpr bool isValidOperand<OpTags::AddWithNumber, Operand1, Operand2> = AddwithNumberInner::Valid<Operand1, Operand2>();
/********************************************************************************************************/


/******************************** AddWithNNumber Aux Params ********************************************************/
template <class Item, class Category>
struct OperAuxParams<OpTags::AddWithNumber, Item, Category> : 
    OperAuxValue<Item> {

    using Base = OperAuxValue<Item>;
    using Base::Base;
    using Base::operator==; 

};
/*********************************************************************************************/



/******************************* add interface both add/addwithnumber ***********************/

template <class Operand1, class Operand2>
constexpr bool isAdditive = isValidOperand<OpTags::Add, Operand1, Operand2> ||
                            isValidOperand<OpTags::AddWithNumber, Operand1, Operand2>;

template <class Operand1, class Operand2>
auto operator+ (Operand1&& op1, Operand2&& op2) {

    if constexpr ( isValidOperand<OpTags::Add, Operand1, Operand2> ) {
        using rawOp1 = RmConstRef<Operand1>;
        using rawOp2 = RmConstRef<Operand2>;

        using ResType = Operation<
            OpTags::Add,
            OperandContainer<rawOp1, rawOp2>
        >;
        return ResType(std::forward<Operand1>(op1), std::forward<Operand2>(op2));

    } else if constexpr ( isValidOperand<OpTags::AddWithNumber, Operand1, Operand2> ) {

        constexpr bool isCate1 = isValidCategory<DataCategory<Operand1>>;
        constexpr bool isCate2 = isValidCategory<DataCategory<Operand2>>;

        if constexpr (isCate1 && !isCate2) {

            using rawOp = RmConstRef<Operand1>;
            using ResType = Operation<
                OpTags::AddWithNumber,
                OperandContainer<rawOp>
            >;
            OperAuxParams<
                OpTags::AddWithNumber,
                typename ResType::ItemType,
                typename ResType::CategoryTag
            > param(op2);

            return ResType ( 
                std::move(param),
                std::forward<Operand1>(op1)
            );

        } else if constexpr (!isCate1 && isCate2) {
            
            using rawOp = RmConstRef<Operand2>;
            using ResType = Operation<
                OpTags::AddWithNumber,
                OperandContainer<rawOp>
            >;
            OperAuxParams<
                OpTags::AddWithNumber,
                typename ResType::ItemType,
                typename ResType::CategoryTag
            > param(op1);

            return ResType ( 
                std::move(param),
                std::forward<Operand2>(op2)
            );

        } else {
            static_assert(DependencyFalse<Operand1, Operand2>);
        }
    } else {
        static_assert(DependencyFalse<Operand1, Operand2>);
    }

}

/*********************************************************************************************/

};