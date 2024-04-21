#pragma once
#include <atomic>

namespace JYNN{

class InstanceID{
    inline static std::atomic<size_t> counter_;
public:
    InstanceID() = delete;
    static size_t Get() {
        return counter_.fetch_add(1);
    }
};

};