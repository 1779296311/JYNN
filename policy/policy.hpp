#pragma once

#include "policy/policyContainer.hpp"

namespace JYNN {

//help
template <class PolicyA, class PolicyB>
constexpr bool isSamePolicy = std::is_same_v<typename PolicyA::MajorClass, typename PolicyB::MajorClass> &&
                              std::is_same_v<typename PolicyA::MinorClass, typename PolicyB::MinorClass> ;


/********************************************** select common MajorClass *******************************************************************/
namespace PolicySelectInner{
template<class MajorClass>
struct SameMajor {
    template <class Cont, class Item>
    using Fun = std::conditional<
                    std::is_same_v<MajorClass, typename Item::MajorClass>,
                    Sequential::PushBk<Cont, Item>,
                    Cont
                    >;
};
struct DifferentMinor {
    template <class PolicyA, class PolicyB>
    struct FunImp_ {
        static constexpr bool value = 
            !std::is_same_v<typename PolicyA::MinorClass, typename PolicyB::MinorClass>;
    };
    template <class PolicyA, class PolicyB>
    using Fun = FunImp_<PolicyA, PolicyB>;
};

template <class Policies>
struct MakePolicy;
template <class Cur, class... Items, template <class...> class Cont>
struct MakePolicy<Cont<Cur, Items...>> : 
    virtual Cur, virtual Items...
{};

template <class MajorClass, class Policies>
struct PolicySelector_ {
    using sameMajorPolicies = 
        Sequential::Fliter<PolicyContainer, Policies, SameMajor<MajorClass>::template Fun>;
    constexpr static bool allMinorIsDiff = 
        Sequential::IsUnique<sameMajorPolicies, DifferentMinor::template Fun>;
    static_assert(allMinorIsDiff , "Minor class set conflit!"); 
    using type = 
        std::conditional_t<
            !Sequential::Size<sameMajorPolicies> ,
            MajorClass ,
            MakePolicy<sameMajorPolicies>
        >;
};
}

template <class MajorClass, class Policies>
using PolicySelect = typename PolicySelectInner::PolicySelector_<MajorClass, Policies>::type;
/*******************************************************************************************************************************/



/******************************************************** If exist same Ma/mi Class **************************************************/
namespace PolicyExistInner{
    template <class PCont, class MajorClass, class MinorClass>
    struct PolicyExist_;
    template <template <class...> class Cont, class MajorClass, class MinorClass>
    struct PolicyExist_<Cont<>, MajorClass, MinorClass> {
        static constexpr bool value = false;
    };
    template <class Cur, class... Items, template <class...> class Cont, class MajorClass, class MinorClass> 
    struct PolicyExist_<Cont<Cur, Items...>, MajorClass, MinorClass> {
        static constexpr bool curOk = 
            std::is_same_v<typename Cur::MajorClass, MajorClass> && std::is_same_v<typename Cur::MinorClass, MinorClass>;
        static constexpr bool value = curOk ? true : PolicyExist_<Cont<Items...>, MajorClass, MinorClass>::value ;
    };
}

template <class PCont, class MajorClass, class MinorClass>
constexpr bool PolicyExist = PolicyExistInner::PolicyExist_<PCont, MajorClass, MinorClass>::value;
/*******************************************************************************************************************************/



/*************************************************** Get first policy by Ma/Mi Class ****************************************************************************/
namespace PolicyGetInner{
    template <class PCont, class MajorClass, class MinorClass>
    struct PolicyGet_;
    template <template <class...> class Cont, class MajorClass, class MinorClass>
    struct PolicyGet_<Cont<>, MajorClass, MinorClass> :
        std::enable_if<true, MajorClass>
    {};
    template <class Cur, class... Items, template <class...> class Cont, class MajorClass, class MinorClass> 
    struct PolicyGet_<Cont<Cur, Items...>, MajorClass, MinorClass> :
        std::conditional_t<
            std::is_same_v<typename Cur::MajorClass, MajorClass> && std::is_same_v<typename Cur::MinorClass, MinorClass> ,
            std::enable_if<true, Cur>,
            PolicyGet_<Cont<Items...>, MajorClass, MinorClass>
        >
    {};
}

template <class PCont, class MajorClass, class MinorClass>
using PolicyGet = typename PolicyGetInner::PolicyGet_<PCont, MajorClass, MinorClass>::type;
/*******************************************************************************************************************************/


/********************************************** Policy change ***********************************************/
namespace PolicyChangeInnet{
template <class NewPolicy>
struct Imp{

    template <class ResCont, class Cur>
    struct Fun : 
        std::conditional<
            isSamePolicy<Cur, NewPolicy>,
            ResCont,
            Sequential::PushBk<ResCont, Cur>
        >
    {};

    template <class ResCont, class LayerName, class... Policies>
    struct Fun<ResCont, SubPolicyContainer<LayerName, Policies...>> : 
        std::enable_if<
            true, 
            Sequential::PushBk<ResCont, SubPolicyContainer<LayerName, Policies...>>
        >
    {};
};
};

template <class NewPolicy, class PolicyCont>
using ChangePolicy = 
    Sequential::PushBk<
        Sequential::Fliter<PolicyContainer, PolicyCont, PolicyChangeInnet::Imp<NewPolicy>::template Fun>,
        NewPolicy
    >;
/*******************************************************************************************************************************/



/********************************************** Set substitude(exist) or push_back ***********************************************/
namespace PolicySetInner{
    template <class LCont, class RCont, class Policy>
    struct PolicySet_;
    template <class... L, template <class...> class LCont, template <class...> class RCont, class Policy>
    struct PolicySet_<LCont<L...>, RCont<>, Policy> :
        std::enable_if<true, LCont<Policy, L...>>
    {};
    template <class... L, template <class...> class LCont, class Cur, class... R, template <class...> class RCont, class Policy>
    struct PolicySet_<LCont<L...>, RCont<Cur, R...>, Policy> : 
        std::conditional_t<
            isSamePolicy<Policy, Cur>,
            std::enable_if<true, LCont<L..., Policy, R...>>,
            PolicySet_<LCont<L..., Cur>, RCont<R...>, Policy>
        >
    {};
    template <class Cont, class Policy>
    struct PolicySetImp_;
    template <class... Items, template <class...> class Cont, class Policy>
    struct PolicySetImp_<Cont<Items...>, Policy> :
        PolicySet_<Cont<>, Cont<Items...>, Policy>
    {};
}

template <class Cont, class Policy>
using PolicySet = typename PolicySetInner::PolicySetImp_<Cont, Policy>::type;
/*******************************************************************************************************************************/



/****************************************** Sub from L to R *********************************************************************/
namespace PolicySubInner{
    template <class A, class B>
    struct Pair{
        using first_type = A;
        using second_type = B;
    }; 
    template <class Pick, class Remain, class Cont, size_t L, size_t R, size_t i>
    struct PolicySub_;

    template <class Picked, class Left,
              template <class...> class Cont, 
              size_t L, size_t R, size_t i>
    struct PolicySub_<Picked, Left, Cont<>, L, R, i> :
        std::enable_if<true, Pair<Picked, Left>>
    {};

    template <class... pItems,  template <class...> class Picked,
              class... lItems,  template <class...> class Left,
              class Cur, class... Items, template <class...> class Cont, 
              size_t L, size_t R, size_t i>
    struct PolicySub_<Picked<pItems...>, Left<lItems...>, Cont<Cur, Items...>, L, R, i> :
        std::conditional_t<
            ( i >= L && i < R ) ,
            PolicySub_<Picked<pItems..., Cur>, Left<lItems...>, Cont<Items...>, L, R, i+1>,
            PolicySub_<Picked<pItems...>, Left<lItems..., Cur>, Cont<Items...>, L, R, i+1>
        >
    {};

    template <class Cont, size_t L, size_t R>
    struct PolicySubImp_;
    template <class... Items, template <class...> class Cont, size_t L, size_t R>
    struct PolicySubImp_<Cont<Items...>, L, R> : 
        PolicySub_<Cont<>, Cont<>, Cont<Items...>, L, R, 0>
    {};

}

template <class Cont, size_t L, size_t R>
using PolicySub = typename PolicySubInner::PolicySubImp_<Cont, L, R>::type;
/*******************************************************************************************************************************/



/******************************************** except SubPolicyContainer in PolicyContainer********************************************/
namespace PlainPolicyInner {
struct Imp_ {

    template <class ResCont, class Cur>
    struct Fun :
        std::enable_if<true, Sequential::PushBk<ResCont, Cur>>
    {};

    template <class ResCont, class LayerName, class... SubPolicies>
    struct Fun<ResCont, SubPolicyContainer<LayerName, SubPolicies...>> : 
        std::enable_if<true, ResCont>
    {};
};
};

template <class PolicyCont>
using PolicyPlain = typename Sequential::Fliter<PolicyContainer, PolicyCont, PlainPolicyInner::Imp_::template Fun>;
/*******************************************************************************************************************************/



/*********************** select policy from B that not exist in A *************************************************************/
namespace PolicyDeriveInner{
template <class PolicyCont, class Policy>
struct Fun : 
    std::conditional<
        !PolicyExist<PolicyCont, typename Policy::MajorClass, typename Policy::MinorClass>,
        Sequential::PushBk<PolicyCont, Policy>,
        PolicyCont
    >
{};
template <class PolicyCont, class LayerName, class... Policies>
struct Fun<PolicyCont, SubPolicyContainer<LayerName, Policies...>> : 
    std::enable_if<true, PolicyCont>
{};
};

template <class PolicyContA, class PolicyContB>
using PolicyDerive = typename Sequential::Fold<PolicyContA, PolicyContB, PolicyDeriveInner::template Fun>::type;
/*****************************************************************************************************************************/



/*************************************  Sub Picker with LayerName *****************************************************/
namespace SubPolicyPickerLayerName {
template <class LayerName>
struct Imp_{

    template <class Res, class Input>
    struct Fun : 
        std::enable_if<true, Res>
    {};

    template <class... ResPolicies, class... SubPolicies>
    struct Fun<PolicyContainer<ResPolicies...>, SubPolicyContainer<LayerName, SubPolicies...>> :
        std::enable_if<true, PolicyContainer<ResPolicies..., SubPolicies...>>
    {};

};

template <class PolicyCont, class LayerName>
struct SubPolicyPicker_{
    using tmp = Sequential::Fliter< PolicyContainer,
                                    PolicyCont,
                                    Imp_<LayerName>::template Fun
                                   >;
   using type = PolicyDerive<tmp, PolicyPlain<PolicyCont>>;
};
};    

template <class PolicyCont, class LayerName>
using SubPolicyPicker = typename SubPolicyPickerLayerName::SubPolicyPicker_<PolicyCont, LayerName>::type;
/*******************************************************************************************************************************/
};