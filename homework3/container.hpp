#include <iostream>
#include <memory>

template <typename T, typename Alloc=std::allocator<T> >
class MyContainer
{
    struct Node
    {
        Node* next = nullptr;
        Node* prev = nullptr;
        T val;
        bool is_nil = false;
    };

    struct Iterator 
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = Node;
        using pointer           = Node*;  // or also value_type*
        using reference         = T&;  // or also value_type&
        Iterator(pointer ptr) : m_ptr(ptr) {};
        reference operator*() const { return (*m_ptr).val; }
        Iterator& operator++() { m_ptr = m_ptr->next; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        Iterator& operator--() { m_ptr = m_ptr->prev; return *this; }
        Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        pointer m_ptr;
    };

public:

    void push_back(const T& val)
    {
        Node* newNode = std::allocator_traits<RebindAlloc>::allocate(nodeAlloc, 1);
        newNode->val = val;
        if(head == nullptr) {
            head = newNode;
            tail = std::allocator_traits<RebindAlloc>::allocate(nodeAlloc, 1);
            tail->is_nil = true;
        }
        else {
            tail->prev->next = newNode;
        }
        newNode->next = tail;
        newNode->prev = tail->prev;
        tail->prev = newNode;
        _size++;
    }
    
    std::size_t size()
    {
        return _size;
    }

    bool empty()
    {
        return (size == 0);
    }

    T& operator [](std::size_t idx) {
        return getByIndex(idx)->val;
    }

    T operator [](std::size_t idx) const {
        
        return getByIndex(idx)->val;
    }

    Iterator begin() { return Iterator(head); }

    Iterator end()   { return Iterator(tail); }

private:
    Node* head = nullptr;
    Node* tail = nullptr;
    std::size_t _size = 0;
    Alloc allocator;
    using RebindAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    RebindAlloc nodeAlloc;

    Node* getByIndex(std::size_t idx)
    {
        if(idx >= _size) {
            throw std::out_of_range("MyContainer");
        }
        Node* temp;
        if(idx < _size / 2) {
            temp = head;
            for(std::size_t i = 0;i < idx;i++, temp = temp->next);
        }
        else {
            temp = tail;
            for(std::size_t i = _size;i > idx;i--, temp = temp->prev);
        }
        return temp;
    }
};