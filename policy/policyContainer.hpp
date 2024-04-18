#pragma once

namespace JYNN{

template <class... Policies>
struct PolicyContainer;

template <class T>
constexpr bool isPolicyCoantiner {false};

template <class... T>
constexpr bool isPolicyCoantiner<PolicyContainer<T...>> {true};

};