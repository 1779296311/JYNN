#pragma once

#include <deque>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <new>
#include <cassert>

#include "data/facilities/device.hpp"

namespace JYNN {

template <class Decive>
struct Allocator;

template <>
struct Allocator<Device::CPU> {  

private:
    inline static std::mutex mutex_;

private:
    struct AllocatorHelper{
        std::unordered_map <size_t, std::deque<void*>> memBuf_;
        ~AllocatorHelper () {
            for(auto &[_, dq] : memBuf_) {
                for(auto &ptr : dq) {
                    char *d = static_cast<char*>(ptr);
                    delete [] d;
                }
                dq.clear();
            }
        }
    };

public:
    template <class T, size_t alignBits = 6>
    static std::shared_ptr<T> Allocate(size_t itemN) {
        if (itemN == 0) {
            return nullptr;
        }
        size_t byteSize = itemN * sizeof(T);

        static constexpr size_t align = (1 << alignBits);
        if(byteSize & (align - 1)) {
            byteSize = ( (byteSize >> alignBits) + 1 ) << alignBits;
        }

        assert( !(byteSize & (align - 1)) );

        std::unique_lock<std::mutex> lock(mutex_);

        static AllocatorHelper allocatorHelper;
        auto &memdq = allocatorHelper.memBuf_[byteSize];
        void* ptr {nullptr};

        if(memdq.empty()) {
            ptr = new char[byteSize];
        }else{
            ptr = memdq.back();
            memdq.pop_back();
        }

        assert(ptr != nullptr);

        return std::shared_ptr<T>(
            static_cast<T*>(ptr),
            [&memdq](void *p) {
                std::unique_lock<std::mutex> lock(mutex_);
                memdq.emplace_back(p);
            }
        );
    }

    template <class T, class... Args>
    static void Construct(std::shared_ptr<T> ptr, Args&&... args) {
        assert(ptr != nullptr);
        new (ptr.get()) T(std::forward<Args>(args)...);
    } 



};

};