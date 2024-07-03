#pragma once

#include <assert.h>
#include <cstdint>
#include <cstring>
#include <memory>

template <class T, std::size_t N = 1, std::size_t POOL_SIZE = 10>
struct pool_allocator {

    using value_type = T;

    std::size_t current_capacity = 0;
    std::size_t num_pools = 0;
    std::size_t MAX_NUM_POOL = 100;
    std::size_t size = sizeof(T) * POOL_SIZE;
    uint8_t **data = static_cast<uint8_t**>(::operator new(MAX_NUM_POOL));

    std::size_t size_chunks = 0;
    std::size_t free_chunks_count = 0;
    std::size_t* free_chunks = static_cast<std::size_t*>(::operator new(size_chunks));

    void add_new_pool()
    {
        assert(num_pools < MAX_NUM_POOL);
        std::size_t old_size_chunks = size_chunks;
        std::size_t old_free_chunks_count = free_chunks_count;
        
        current_capacity+= POOL_SIZE;
        num_pools++;
        data[num_pools - 1] = static_cast<uint8_t*>(::operator new(size));

        size_chunks = sizeof(T*) * current_capacity;
        free_chunks_count+= POOL_SIZE / N;
        std::size_t* new_free_chunks = static_cast<std::size_t*>(::operator new(size_chunks));
        std::memcpy(new_free_chunks, free_chunks, old_size_chunks);
        delete(free_chunks);
        free_chunks = new_free_chunks;
        for(std::size_t i = 0;i < POOL_SIZE;i++) {
            free_chunks[old_free_chunks_count + i] = reinterpret_cast<std::size_t>(data[num_pools - 1] + i * sizeof(T));
        }
    }

    bool in_pool(T* p, std::size_t n)
    {
        for(std::size_t i = 0;i < num_pools;i++) {
            if((p >= reinterpret_cast<T*>(data[num_pools - 1])) && (p + n <= reinterpret_cast<T*>(data[num_pools - 1] + size))) {
                if((reinterpret_cast<uint8_t*>(p) - data[num_pools - 1]) % (sizeof(T) * N) == 0) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        return false;
    }

    pool_allocator () noexcept
    {
        assert(POOL_SIZE % N == 0);
        add_new_pool();
    }

    template <class U> pool_allocator(const pool_allocator <U>& a) noexcept {
        current_capacity = a.current_capacity;
        num_pools = a.num_pools;
        MAX_NUM_POOL = a.MAX_NUM_POOL;
        size = a.size;
        data = a.data;

        size_chunks = a.size_chunks;
        free_chunks_count = a.free_chunks_count;
        free_chunks = a.free_chunks;
    }

    pool_allocator select_on_container_copy_construction() const
    { 
        return pool_allocator();
    }

    T* allocate (std::size_t n)
    {
        assert(n == N);
        if(free_chunks_count == 0) {
            add_new_pool();
        }
        free_chunks_count--;
        return reinterpret_cast<T*>(free_chunks[free_chunks_count]);
    }

    void deallocate (T* p, std::size_t n)
    {
        std::cout << "Deallocate " << n << std::endl;
        assert(n == N);
        assert(free_chunks_count < current_capacity / N);
        assert(in_pool(p, n));
        free_chunks[free_chunks_count] = reinterpret_cast<std::size_t>(p);
        free_chunks_count++;
        delete(p);
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

template <class T, class U, std::size_t N = 1, std::size_t POOL_SIZE = 10>
constexpr bool operator== (const pool_allocator<T, N, POOL_SIZE>& a1, const pool_allocator<U, N, POOL_SIZE>& a2) noexcept
{
    return (a1.data == a2.data);
}

template <class T, class U, std::size_t N = 1, std::size_t POOL_SIZE = 10>
constexpr bool operator!= (const pool_allocator<T, N, POOL_SIZE>& a1, const pool_allocator<U, N, POOL_SIZE>& a2) noexcept
{
    return (a1.data != a2.data);
}
