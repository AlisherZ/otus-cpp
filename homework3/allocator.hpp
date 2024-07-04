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
    T **data = static_cast<T**>(malloc(sizeof(T *) * MAX_NUM_POOL));//static_cast<uint8_t**>(::operator new(MAX_NUM_POOL));

    std::size_t free_chunks_count = 0;
    void **free_chunks = nullptr;

    void print_state()
    {
        std::cout << "State of Allocator: " << std::endl;
        std::cout << current_capacity << std::endl;
        std::cout << num_pools << std::endl;
        std::cout << free_chunks_count << std::endl;
        std::cout << sizeof(free_chunks[0]) << std::endl;
        std::cout << sizeof(T) << std::endl;
        std::cout << sizeof(T*) << std::endl;
        std::cout << "State of data: " << std::endl;
        for(std::size_t i = 0;i < num_pools;i++) {
            std::cout << "Address " << static_cast<void *>(data[i]) << " Object Address " << (data[i]) << std::endl;
            for(std::size_t j = 0;j < sizeof(T) * POOL_SIZE;j++) {
                //std::cout << "Value " << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "State of chunks: " << std::endl;
        for(std::size_t i = 0;i < free_chunks_count;i++) {
            std::cout << "Address " << free_chunks[i] << " Object Address " << static_cast<T*>(free_chunks[i]) << std::endl;
            std::cout << "Value " << (int)(*(reinterpret_cast<uint8_t *>(free_chunks[i]))) << std::endl;
        }
        std::cout << "_________________________________" << std::endl;
    }

    void add_new_pool()
    {
        assert(num_pools < MAX_NUM_POOL);
        std::size_t old_current_capacity = current_capacity;
        std::size_t old_free_chunks_count = free_chunks_count;
        
        current_capacity+= POOL_SIZE;
        num_pools++;
        T* _pool = static_cast<T*>(malloc(sizeof(T) * POOL_SIZE));
        data[num_pools - 1] = _pool;//static_cast<T*>(_pool);//static_cast<uint8_t*>(malloc(sizeof(T) * POOL_SIZE));//static_cast<uint8_t*>(::operator new(size));

        free_chunks_count+= POOL_SIZE / N;
        void **new_free_chunks = static_cast<void **>(malloc(sizeof(T *) * current_capacity));//static_cast<std::size_t*>(::operator new(size_chunks));
        if(old_current_capacity > 0) {
            std::memcpy(new_free_chunks, free_chunks, sizeof(T *) * old_current_capacity);
            delete(free_chunks);
        }
        free_chunks = new_free_chunks;
        for(std::size_t i = 0;i < POOL_SIZE;i++) {
            free_chunks[old_free_chunks_count + i] = (data[num_pools - 1] + i);
        }
        //print_state();
    }

    bool in_pool(T* p, std::size_t n)
    {
        for(std::size_t i = 0;i < num_pools;i++) {
            if((p >= data[i]) && (p + n <= data[i] + POOL_SIZE)) {
                if((reinterpret_cast<uint8_t*>(p) - reinterpret_cast<uint8_t*>(data[i])) % (sizeof(T) * N) == 0) {
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
        //data = static_cast<T**>(a.data);
        data = static_cast<T**>(malloc(sizeof(T *) * MAX_NUM_POOL));
        std::memcpy(data, a.data, sizeof(T *) * MAX_NUM_POOL);

        free_chunks_count = a.free_chunks_count;
        free_chunks = a.free_chunks;
    }

    pool_allocator select_on_container_copy_construction() const
    { 
        return pool_allocator();
    }

    T* allocate (std::size_t n)
    {
        //std::cout << "Try allocate " << n << std::endl;
        //print_state();
        assert(n == N);
        if(free_chunks_count == 0) {
            add_new_pool();
        }
        free_chunks_count--;
        //print_state();
        //std::cout << "Allocate " << n << std::endl;
        //T *test = static_cast<T*>(free_chunks[free_chunks_count]);
        //*test = test1;
        //T test2 = *test;
        return static_cast<T*>(free_chunks[free_chunks_count]);
        //return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate (T* p, std::size_t n)
    {
        //std::cout << "Deallocate " << n << std::endl;
        assert(n == N);
        assert(free_chunks_count < current_capacity / N);
        assert(in_pool(p, n));
        free_chunks[free_chunks_count] = static_cast<void *>(p);
        free_chunks_count++;
        //delete(p);
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
