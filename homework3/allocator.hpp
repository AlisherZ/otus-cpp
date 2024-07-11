#pragma once

#include <assert.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <utility>

template <class T, std::size_t CHUNK_SIZE = 1, std::size_t POOL_SIZE = 10>
struct pool_allocator
{
    using pool_type = std::shared_ptr<T[]>;
    using chunk_type = T*;
    using value_type = T;

    std::size_t current_capacity = 0;
    std::size_t num_pools = 0;
    std::size_t MAX_NUM_POOL = 100;
    std::shared_ptr<pool_type[]> data = std::shared_ptr<pool_type[]>(new pool_type[MAX_NUM_POOL]);

    std::size_t free_chunks_count = 0;
    std::size_t chunks_capacity = 0;
    std::shared_ptr<chunk_type[]> free_chunks;

    void add_new_pool()
    {
        assert(num_pools < MAX_NUM_POOL);
        std::size_t old_current_capacity = current_capacity;
        std::size_t old_free_chunks_count = free_chunks_count;
        
        current_capacity+= POOL_SIZE;
        num_pools++;
        data[num_pools - 1] = std::shared_ptr<T[]>(new T[POOL_SIZE]);

        free_chunks_count+= POOL_SIZE / CHUNK_SIZE;
        chunks_capacity+= POOL_SIZE / CHUNK_SIZE;
        auto new_free_chunks = std::shared_ptr<chunk_type[]>(new chunk_type[chunks_capacity]);
        if(old_current_capacity > 0) {
            std::memcpy(new_free_chunks.get(), free_chunks.get(), sizeof(chunk_type) * old_free_chunks_count);
        }
        free_chunks = new_free_chunks;
        for(std::size_t i = 0, j = 0;j < POOL_SIZE;i++,j+= CHUNK_SIZE) {
            free_chunks[old_free_chunks_count + i] = &data[num_pools - 1][j];
        }
    }

    bool in_pool(T* p, std::size_t n)
    {
        for(std::size_t i = 0;i < num_pools;i++) {
            if((p >= &data[i][0]) && ((p + n) <= (&data[i][0] + POOL_SIZE))) {
                if((reinterpret_cast<uint8_t*>(p) - reinterpret_cast<uint8_t*>(&data[i][0])) % (sizeof(T) * CHUNK_SIZE) == 0) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        return false;
    }

    pool_allocator() noexcept
    {
        assert(POOL_SIZE % CHUNK_SIZE == 0);
        add_new_pool();
    }

    template <class U> pool_allocator(const pool_allocator <U>& a) noexcept {
        current_capacity = a.current_capacity;
        num_pools = a.num_pools;
        MAX_NUM_POOL = a.MAX_NUM_POOL;
        data.reset(a.data.get());
        free_chunks_count = a.free_chunks_count;
        chunks_capacity = a.chunks_capacity;
        free_chunks.reset(a.free_chunks.get());
    }

    pool_allocator select_on_container_copy_construction() const
    { 
        return pool_allocator();
    }

    T* allocate (std::size_t n)
    {
        assert(n == CHUNK_SIZE);
        if(free_chunks_count == 0) {
            add_new_pool();
        }
        free_chunks_count--;
        return free_chunks[free_chunks_count];
    }

    void deallocate (T* p, std::size_t n)
    {
        assert(n == CHUNK_SIZE);
        assert(free_chunks_count < chunks_capacity);
        assert(in_pool(p, n));
        free_chunks[free_chunks_count] = p;
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

template <class T, class U, std::size_t CHUNK_SIZE = 1, std::size_t POOL_SIZE = 10>
constexpr bool operator== (const pool_allocator<T, CHUNK_SIZE, POOL_SIZE>& a1, const pool_allocator<U, CHUNK_SIZE, POOL_SIZE>& a2) noexcept
{
    return (a1.data.get() == a2.data.get());
}

template <class T, class U, std::size_t CHUNK_SIZE = 1, std::size_t POOL_SIZE = 10>
constexpr bool operator!= (const pool_allocator<T, CHUNK_SIZE, POOL_SIZE>& a1, const pool_allocator<U, CHUNK_SIZE, POOL_SIZE>& a2) noexcept
{
    return (a1.data.get() != a2.data.get());
}
