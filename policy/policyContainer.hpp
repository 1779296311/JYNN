#pragma once

namespace JYNN{

template <class... Policies>
struct PolicyContainer;

template <class T>
constexpr bool isPolicyCoantiner {false};

template <class... T>
constexpr bool isPolicyCoantiner<PolicyContainer<T...>> {true};

template  <class LayerName, class... Policies>
struct SubPolicyContainer;

template <class T>
constexpr bool isSubPolicyContainer {false};

template <class LayerName, class... Policies>
constexpr bool isSubPolicyContainer<SubPolicyContainer<LayerName, Policies...>> {true};






};