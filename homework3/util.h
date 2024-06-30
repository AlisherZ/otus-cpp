#pragma once

#include <cstdint>
#include <cstring>
#include <memory>

#if defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    #define ADDITIONAL_ALLOCATION 1
#else
    #define ADDITIONAL_ALLOCATION 0
#endif

template <class T, std::size_t N = 10>
struct pool_allocator {

    using value_type = T;

    std::size_t last_allocated = 0;
    static constexpr int size = sizeof(T) * (N + ADDITIONAL_ALLOCATION);
    uint8_t *data;

    pool_allocator () noexcept
    {
        data = static_cast<uint8_t*>(::operator new(size));
    }

    template <class U> pool_allocator(const pool_allocator <U>& a) noexcept {
        data = a.data;
        last_allocated = a.last_allocated;
    }

    pool_allocator select_on_container_copy_construction() const
    { 
        return pool_allocator();
    }

    T* allocate (std::size_t n)
    {
        assert(last_allocated + n <= N + ADDITIONAL_ALLOCATION);
        T* addr = reinterpret_cast<T*>(data) + last_allocated;
        last_allocated+= n;
        return addr;
    }

    void deallocate (T* p, std::size_t n)
    {
        assert(last_allocated >= n);
        assert(p >= reinterpret_cast<T*>(data));
        assert(p + n <= reinterpret_cast<T*>(data) + last_allocated);
        assert((reinterpret_cast<uint8_t*>(p) - data) % sizeof(T) == 0);
        last_allocated-= n;
        std::memmove(p, reinterpret_cast<T*>(data) + last_allocated, n * sizeof(T));
    }

    template< class U >
    struct rebind
    {
        typedef pool_allocator<U> other;
    };

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type; //UB if std::false_type and a1 != a2;
};

template <class T, class U, std::size_t N1 = 10, std::size_t N2 = 10>
constexpr bool operator== (const pool_allocator<T, N1>& a1, const pool_allocator<U, N2>& a2) noexcept
{
    return (a1.data == a2.data);
}

template <class T, class U, std::size_t N1 = 10, std::size_t N2 = 10>
constexpr bool operator!= (const pool_allocator<T, N1>& a1, const pool_allocator<U, N2>& a2) noexcept
{
    return (a1.data != a2.data);
}
