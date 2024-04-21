#pragma once


namespace JYNN{


template <class T>
using RmConstRef = std::remove_cv_t<std::remove_reference_t<T>>;

template <class ...>
constexpr static bool DependencyFalse {false};

};