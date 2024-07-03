#include <iostream>
#include <memory>

template <typename T, typename Alloc=std::allocator<T>>
class MyContainer
{
    struct Node
    {
        Node* next = nullptr;
        Node* prev = nullptr;
        T val;
    };
public:
    void push_back(const T& val)
    {
        //Node* newNode = allocator.allocate(1); //allocator is of type std_03_allocator<int>!
        typename Alloc::template rebind<Node>::other nodeAlloc;
        Node* newNode = nodeAlloc.allocate(1);
        newNode->val = val;
        if(head == nullptr) {
            head = newNode;
            tail = newNode;
            _size = 1;
            return;
        }
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
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
    

private:
    Node* head = nullptr;
    Node* tail = nullptr;
    std::size_t _size = 0;
    Alloc allocator;

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
            for(std::size_t i = _size - 1;i > idx;i--, temp = temp->prev);
        }
        return temp;
    }
};