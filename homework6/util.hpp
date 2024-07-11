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

template <typename T, T value, typename LevelType>
class MatrixLineIterator : public MatrixZip<T, value, LevelType>
{
	using BaseClass = MatrixZip<T, value, LevelType>;
public:
	auto getFirst();
	auto getLast();
	auto curBegin() { return BaseClass::begin(); };
	auto curEnd()   { return BaseClass::end(); };
};

template <typename T, T value, typename LevelType = T>
class Node
{
	using NextLevelType = MatrixLineIterator<T, value, LevelType>;
	using CurrentIterator = typename std::map<std::size_t, LevelType>::iterator;
public:
	Node() {};
	Node(NextLevelType* ctx) : context(ctx), it(ctx->curBegin()) {};
	Node(NextLevelType* ctx, CurrentIterator it_1) : context(ctx), it(it_1) {};
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

template <typename T, T value, typename PrevLevelType>
class Node<T, value, MatrixLineIterator<T, value, PrevLevelType> >
{
	using LevelType = MatrixLineIterator<T, value, PrevLevelType>;
	using NextLevelType = MatrixLineIterator<T, value, LevelType>;
	using CurrentIterator = typename std::map<std::size_t, LevelType>::iterator;
public:
	Node() {};
	Node(NextLevelType* ctx) : context(ctx), it(ctx->curBegin()) {
		if(ctx->curBegin() != ctx->curEnd()) {
			it1 = ctx->curBegin()->second.getFirst();
		}
	};
	Node(NextLevelType* ctx, CurrentIterator it_1) : context(ctx), it(it_1) {
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
	Node<T, value, PrevLevelType> it1;
	NextLevelType* context;
};

template <typename T, T value, typename LevelType>
auto MatrixLineIterator<T, value, LevelType>::getFirst() {
	return Node<T, value, LevelType>(this);
}

template <typename T, T value, typename LevelType>
auto MatrixLineIterator<T, value, LevelType>::getLast() {
	return Node<T, value, LevelType>(this, curEnd());
}

template <typename T, T value>
using Row = MatrixLineIterator<T, value, T>;

template <typename T, T value>
using Matrix2D = MatrixLineIterator<T, value, Row<T, value> >;
