#include <map>
#include <vector>
#include <string>
#include <iostream>

template <typename T, T value>
class Row
{
public:
	struct Node
	{
		Node(Row* enc, std::size_t i, T val) {
			enclosure = enc;
			value->first = i;
			value->second = val;
		}
		Node(Row* enc, bool isFinal = false) {
			enclosure = enc;
			if(auto it = enc->cells.begin(); (it != enc->cells.end()) && !isFinal) {
				value.first = it->first;
				value.second = it->second;
			}
			else {
				isEnd = true;
			}
		}
		bool hasNext() {
			return !isEnd;
		}
		Node& next() {
			auto it = enclosure->cells.find(value.first);
			it++;
			if(it != enclosure->cells.end()) {
				value.first = it->first;
				value.second = it->second;
			}
			else {
				isEnd = true;
			}
			return *this;
		}
		Row* enclosure;
		std::pair<std::size_t, T> value;
		bool isEnd = false;
	};
	struct Iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = Node;
		using pointer           = std::shared_ptr<Node>;  // or also value_type*
		using reference         = std::pair<std::size_t, T>&;  // or also value_type&
		Iterator(pointer ptr) : m_ptr(std::move(ptr)) {};
		reference operator*() const { return m_ptr->value; }
		pointer operator->() { return m_ptr; }
		Iterator& operator++() { if(m_ptr->hasNext()) m_ptr->next(); return *this; }
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		friend bool operator== (const Iterator& a, const Iterator& b) { if(a.m_ptr->isEnd && b.m_ptr->isEnd) return true; return a.m_ptr == b.m_ptr; };
		friend bool operator!= (const Iterator& a, const Iterator& b) { if(a.m_ptr->isEnd && b.m_ptr->isEnd) return false; return a.m_ptr != b.m_ptr; };

	private:
		pointer m_ptr;
	};
	Row() {};
	std::size_t size() {
		return cells.size();
	}
	T& operator [](std::size_t idx) {
		if(!contain(idx)){
			cells[idx] = value;
		}
		return cells[idx];
	}
	T operator [](std::size_t idx) const {
		if(!contain(idx)){
			cells[idx] = value;
		}
		return cells[idx];
	}
	bool contain(std::size_t idx) {
		return cells.find(idx) != cells.end();
	}
	void erase(std::size_t idx) {
		if(auto it = cells.find(idx); cells.find(idx) != cells.end()) {
			cells.erase(it);
		}
	}
	void invalidate() {

	}
	
    Iterator begin() { return Iterator(std::make_shared<Node>(Node(this))); }

    Iterator end()   { return Iterator(std::make_shared<Node>(Node(this, true))); }
private:
	std::map<std::size_t, T> cells;
};
