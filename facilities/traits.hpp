#pragma once


namespace JYNN{


template <class T>
using RmConstRef = std::remove_cv_t<std::remove_reference_t<T>>;

template <class ...>
constexpr static bool DependencyFalse {false};


template <class TBooleanCont, class TFunCont>
struct CompileTimeSwitch_;

template <bool curBool, bool... TBools,
          template<class...> class TFunCont, class curFunc, class... TFuncs>
struct CompileTimeSwitch_<std::integer_sequence<bool, curBool, TBools...>, TFunCont<curFunc, TFuncs...>> {
    static_assert((sizeof...(TBools) == sizeof...(TFuncs)) ||
                  (sizeof...(TBools) + 1 == sizeof...(TFuncs)));
    using type = class std::conditional_t<curBool,
                                          std::enable_if<true, curFunc>,
                                          CompileTimeSwitch_<std::integer_sequence<bool, TBools...>, TFunCont<TFuncs...>>>::type;
};

template <template<class...> class TFunCont, class curFunc>
struct CompileTimeSwitch_<std::integer_sequence<bool>, TFunCont<curFunc>> :
    std::enable_if<true, curFunc>
{};

template <class TBooleanCont, class TFunCont>
using CompileTimeSwitch = class CompileTimeSwitch_<TBooleanCont, TFunCont>::type;


};