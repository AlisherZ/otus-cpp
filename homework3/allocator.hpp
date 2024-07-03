#pragma once

#include <assert.h>
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

    std::size_t* free_chunks = static_cast<std::size_t*>(::operator new(size_chunks));
    std::size_t chunk_count_to_allocate = 1;
    std::size_t free_chunks_count = (N + ADDITIONAL_ALLOCATION) / chunk_count_to_allocate;
    std::size_t last_allocated = 0;
    static constexpr int size_chunks = sizeof(T*) * (N + ADDITIONAL_ALLOCATION);
    static constexpr int size = sizeof(T) * (N + ADDITIONAL_ALLOCATION);
    uint8_t *data = static_cast<uint8_t*>(::operator new(size));

    pool_allocator () noexcept
    {
        for(std::size_t i = 0;i < N + ADDITIONAL_ALLOCATION;i++) {
            free_chunks[i] = reinterpret_cast<size_t>(data + i * sizeof(T));
        }
    }

    template <class U> pool_allocator(const pool_allocator <U>& a) noexcept {
        data = a.data;
        last_allocated = a.last_allocated;
        free_chunks = a.free_chunks;
    }

    pool_allocator select_on_container_copy_construction() const
    { 
        return pool_allocator();
    }

    T* allocate (std::size_t n)
    {
        assert(n == chunk_count_to_allocate);
        assert(free_chunks_count > 0);
        free_chunks_count--;
        return reinterpret_cast<T*>(free_chunks[free_chunks_count]);
    }

    void deallocate (T* p, std::size_t n)
    {
        assert(n == chunk_count_to_allocate);
        assert(free_chunks_count < (N + ADDITIONAL_ALLOCATION) / chunk_count_to_allocate);
        assert(p >= reinterpret_cast<T*>(data));
        assert(p + n <= reinterpret_cast<T*>(data) + size);
        assert((reinterpret_cast<uint8_t*>(p) - data) % (sizeof(T) * chunk_count_to_allocate) == 0);
        free_chunks[free_chunks_count] = reinterpret_cast<std::size_t>(p);
        free_chunks_count++;
    }

    template< class U >
    struct rebind
    {
        typedef pool_allocator<U> other;
    };

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
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
