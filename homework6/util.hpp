#include <map>
#include <memory>
#include <string>
#include <vector>

template <typename... T>
struct is_idx_type : std::conjunction<std::is_same<std::size_t, T> ...> {};

template <typename... T>
inline constexpr bool is_idx_type_v = is_idx_type<T...>::value;

template <typename Head, typename... Tail>
std::tuple<Tail...> tuple_tail(const std::tuple<Head, Tail...>& t)
{
    return std::apply([](auto head, auto... tail) {
        return std::make_tuple(tail...);
    }, t);
}

template <class... T>
std::tuple<T...> tuple_cmp(std::tuple<T...> lt, std::tuple<T...> rt)
{
	if(std::get<0>(lt) != std::get<0>(rt)) {
		return false;
	}
	if constexpr(sizeof...(T) > 1) {
    	return tuple_cmp(tuple_tail(lt), tuple_tail(rt));
	}
	else {
		return true;
	}
}

template <typename T, T value, typename LevelType>
class MatrixBase
{
public:
	MatrixBase() {};
	std::size_t size() {
		if constexpr(std::is_same_v<T, LevelType>) {
			return rows.size();
		}
		else {
			std::size_t sz = 0;
			for(auto it = rows.begin();it != rows.end();it++) {
				sz+= it->second.size();
			}
			return sz;
		}
	}
	auto& operator [](std::size_t idx) {
		if(!contain(idx)){
			if constexpr(!std::is_same_v<T, LevelType>) {
				rows[idx] = LevelType();
			}
			else {
				rows[idx] = value;
			}
		}
		return rows[idx];
	}
	auto operator [](std::size_t idx) const {
		if(!contain(idx)){
			if constexpr(!std::is_same_v<T, LevelType>) {
				rows[idx] = LevelType();
			}
			else {
				rows[idx] = value;
			}
		}
		return rows[idx];
	}
	bool contain(std::size_t idx) {
		return rows.find(idx) != rows.end();
	}
	template <typename ...Args>
	typename std::enable_if_t<is_idx_type_v<Args...>, bool> contain(std::tuple<Args...> id) {
		if(auto it = rows.find(std::get<0>(id)); it != rows.end()) {
			if constexpr(sizeof...(Args) > 1) {
				it->second.contain(tuple_tail(id));
			}
			else {
				return true;
			}
		}
		else {
			return false;
		}
	}
	template <typename ...Args>
	typename std::enable_if_t<is_idx_type_v<Args...>, bool> erase(std::tuple<Args...> id) {
		if(auto it = rows.find(std::get<0>(id)); it != rows.end()) {
			if constexpr(sizeof...(Args) > 1) {
				it->second.erase(tuple_tail(id));
				if(it->second.size() == 0) {
					rows.erase(it);
				}
			}
			else {
				rows.erase(it);
			}
		}
	}
	auto begin() { return rows.begin(); }
	auto end()   { return rows.end(); }
protected:
	std::map<std::size_t, LevelType> rows;
};

template <typename T, T value, typename LevelType>
class MatrixZip : public MatrixBase<T, value, LevelType>
{
	using BaseClass = MatrixBase<T, value, LevelType>;
public:
	void invalidate() {
		for (auto it = BaseClass::rows.begin(); it != BaseClass::rows.end();)
		{
			bool need_erase;
			if constexpr(!std::is_same_v<T, LevelType>) {
				it->second.invalidate();
				need_erase = (it->second.size() == 0);
			}
			else {
				need_erase = (it->second == value);
			}
			if (need_erase) {
				it = BaseClass::rows.erase(it);
			}
			else {
				++it;
			}
		}
	}
	std::size_t size() {
		invalidate();
		return BaseClass::size();
	}
	auto begin() { invalidate(); return BaseClass::begin(); }
	auto end()   { invalidate(); return BaseClass::end(); }
};

template <typename T, T value, typename LevelType = T>
class MatrixLineIterator : public MatrixZip<T, value, LevelType>
{
	using BaseClass = MatrixZip<T, value, LevelType>;
public:
	class Node
	{
		using NextLevelType = MatrixLineIterator<T, value, LevelType>;
		using CurrentIterator = typename std::map<std::size_t, LevelType>::iterator;
	public:
		Node() : context(nullptr) {};
		Node(NextLevelType* ctx) : it(ctx->curBegin()), context(ctx) {};
		Node(NextLevelType* ctx, CurrentIterator it_1) : it(it_1), context(ctx) {};
		bool hasNext() {
			return it != context->curEnd();
		}
		void next() {
			if(hasNext()) {
				it++;
			}
		}
		auto getIters() {
			return std::tie(it->first);
		}
		T& getValue() {
			return it->second;
		}
		bool operator==(const Node& rhs) const {
			return it == rhs.it;
		}
		bool operator!=(const Node& rhs) const {
			return it != rhs.it;
		}
	private:
		CurrentIterator it;
		NextLevelType* context;
	};
	struct Iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = Node;
		using pointer           = std::shared_ptr<Node>;  // or also value_type*
		using reference         = std::pair<std::size_t, T>&;  // or also value_type&
		Iterator(pointer ptr) : m_ptr(ptr) {};
		auto operator*() const { return std::tuple_cat(m_ptr->getIters(), std::tie(m_ptr->getValue())); };
		pointer operator->() { return m_ptr; };
		Iterator& operator++() { m_ptr->next(); return *this; };
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; };
		friend bool operator== (const Iterator& a, const Iterator& b) { return *(a.m_ptr) == *(b.m_ptr); };
		friend bool operator!= (const Iterator& a, const Iterator& b) { return *(a.m_ptr) != *(b.m_ptr); };

	private:
		pointer m_ptr;
	};
	auto getFirst() { return Node(this); };
	auto getLast() { return Node(this, curEnd()); };
	auto curBegin() { return BaseClass::begin(); };
	auto curEnd()   { return BaseClass::end(); };
	Iterator begin() { return Iterator(std::make_shared<Node>(this)); }
	Iterator end()   { return Iterator(std::make_shared<Node>(this, curEnd())); }
};

template <typename T, T value, typename PrevLevelType>
class MatrixLineIterator<T, value, MatrixLineIterator<T, value, PrevLevelType> > : public MatrixZip<T, value, MatrixLineIterator<T, value, PrevLevelType> >
{
	class Node
	{
		using LevelType = MatrixLineIterator<T, value, PrevLevelType>;
		using NextLevelType = MatrixLineIterator<T, value, LevelType>;
		using CurrentIterator = typename std::map<std::size_t, LevelType>::iterator;
	public:
		Node() : context(nullptr) {};
		Node(NextLevelType* ctx) : it(ctx->curBegin()), context(ctx) {
			if(ctx->curBegin() != ctx->curEnd()) {
				it1 = ctx->curBegin()->second.getFirst();
			}
		};
		Node(NextLevelType* ctx, CurrentIterator it_1) : it(it_1), context(ctx) {
			if(it_1 != ctx->curEnd()) {
				it1 = ctx->curBegin()->second.getFirst();
			}
		};
		bool hasNext() const {
			return it != context->curEnd();
		}
		void next() {
			if(hasNext()) {
				it1.next();
				if(!it1.hasNext()) {
					it++;
					if(hasNext()) {
						it1 = it->second.getFirst();
					}
				}
			}
		}
		auto getIters() {
			return std::tuple_cat(std::tie(it->first), it1.getIters());
		}
		T& getValue() {
			return it1.getValue();
		}
		bool operator==(const Node& rhs) const {
			if((!hasNext()) && (!rhs.hasNext())) {
				return true;
			}
			return (it == rhs.it) && (it1 == rhs.it1);
		}
		bool operator!=(const Node& rhs) const {
			if((!hasNext()) && (!rhs.hasNext())) {
				return false;
			}
			return (it != rhs.it) || (it1 != rhs.it1);
		}
	private:
		CurrentIterator it;
		typename LevelType::Node it1;
		NextLevelType* context;
	};
	struct Iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = Node;
		using pointer           = std::shared_ptr<Node>;  // or also value_type*
		using reference         = std::pair<std::size_t, T>&;  // or also value_type&
		Iterator(pointer ptr) : m_ptr(ptr) {};
		auto operator*() const { return std::tuple_cat(m_ptr->getIters(), std::tie(m_ptr->getValue())); };
		pointer operator->() { return m_ptr; };
		Iterator& operator++() { m_ptr->next(); return *this; };
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; };
		friend bool operator== (const Iterator& a, const Iterator& b) { return *(a.m_ptr) == *(b.m_ptr); };
		friend bool operator!= (const Iterator& a, const Iterator& b) { return *(a.m_ptr) != *(b.m_ptr); };

	private:
		pointer m_ptr;
	};
	using BaseClass = MatrixZip<T, value, MatrixLineIterator<T, value, PrevLevelType>>;
public:
	auto getFirst() { return Node(this); };
	auto getLast() { return Node(this, curEnd()); };
	auto curBegin() { return BaseClass::begin(); };
	auto curEnd()   { return BaseClass::end(); };
	Iterator begin() { return Iterator(std::make_shared<Node>(this)); }
	Iterator end()   { return Iterator(std::make_shared<Node>(this, curEnd())); }
};

template <typename T, T value>
using Row = MatrixLineIterator<T, value, T>;

template <typename T, T value>
using Matrix = MatrixLineIterator<T, value, Row<T, value> >;
