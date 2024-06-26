// #pragma once
// #include <cstddef>


// namespace JYNN::Helper {

// /******************** Int ********************************************/
// template <auto N>
// struct Int_ {
//     constexpr static auto value = N;
// };

// /**********************************************************************/


// /******************** index sequence ********************************************/
// template <size_t... index> struct IndexSequence;

// namespace IndexSequenceInner {
//     template <class L, class R> struct concat;

//     template <size_t... l, size_t... r>
//     struct concat<IndexSequence<l...>, IndexSequence<r...>> : 
//         std::enable_if< true, IndexSequence<l..., (sizeof...(l) + r)...> >
//     {};
// };

// template <size_t N>
// struct MakeIndexSequence_ : 
//     IndexSequenceInner::concat<
//         typename MakeIndexSequence_<N / 2>::type,
//         typename MakeIndexSequence_<N - N/2>::type
//     >
// {};

// template <>
// struct MakeIndexSequence_<1> : 
//     std::enable_if<true, IndexSequence<0>>
// {};
// template <>
// struct MakeIndexSequence_<0> : 
//     std::enable_if<true, IndexSequence<>>
// {};
// template <size_t N>
// using MakeIndexSequence = typename MakeIndexSequence_<N>::type;
// /**********************************************************************************/



// /************************** KV Binder *********************************************/
// template <typename TK, typename TV>
// struct KVBinder {
//     using KeyType = TK;
//     using ValueType = TV;
//     static TV apply(TK*);
// };
// /**********************************************************************************/


// /**********************************************************************************/
// template <typename... TValues>
// struct ValueSequence;
// /**********************************************************************************/

// };


#pragma once

namespace JYNN::Helper
{
// /******************** Int ********************************************/
template <auto N>
struct Int_ {
    constexpr static auto value = N;
};
/**********************************************************************/


// /******************** index sequence ********************************************/
template <int... index> struct IndexSequence;

namespace IndexSequenceInner {
    template <class L, class R> struct concat;

    template <int... l, int... r>
    struct concat<IndexSequence<l...>, IndexSequence<r...>> : 
        std::enable_if< true, IndexSequence<l..., (sizeof...(l) + r)...> >
    {};
};

template <int N>
struct MakeIndexSequence_ : 
    IndexSequenceInner::concat<
        typename MakeIndexSequence_<N / 2>::type,
        typename MakeIndexSequence_<N - N/2>::type
    >
{};

template <>
struct MakeIndexSequence_<1> : 
    std::enable_if<true, IndexSequence<0>>
{};
template <>
struct MakeIndexSequence_<0> : 
    std::enable_if<true, IndexSequence<>>
{};
template <int N>
using MakeIndexSequence = typename MakeIndexSequence_<N>::type;
// /**********************************************************************************/

/************************** KV Binder *********************************************/
template <typename TK, typename TV>
struct KVBinder {
    using KeyType = TK;
    using ValueType = TV;
    static TV apply(TK*);
};
/**********************************************************************************/


/**********************************************************************************/
template <typename... TValues>
struct ValueSequence;
/**********************************************************************************/
}
