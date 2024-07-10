#include <map>
#include <memory>
#include <string>
#include <vector>

template <class... U1>
struct is_idx_type : std::conjunction<std::is_same<std::size_t, U1> ...> {};

template <class... U1>
inline constexpr bool is_idx_type_v = is_idx_type<U1...>::value;

template <typename Head, typename... Tail>
std::tuple<Tail...> tuple_tail(const std::tuple<Head, Tail...>& t)
{
    return std::apply([](auto head, auto... tail) {
        return std::make_tuple(tail...);
    }, t);
}

/*
template <typename T, T value>
class Row
{
	using Iterator = typename std::map<std::size_t, T>::iterator;
public:
	Row() {};
	std::size_t size() {
		//invalidate();
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
		if(auto it = cells.find(idx); it != cells.end()) {
			cells.erase(it);
		}
	}
	void invalidate() {
		for (auto it = cells.cbegin(); it != cells.cend();)
		{
			if ((*it).second == -1)
			{
				it = cells.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	
	Iterator begin() { invalidate(); return cells.begin(); }

	Iterator end()   { invalidate(); return cells.end(); }
private:
	std::map<std::size_t, T> cells;
};
*/

template <typename LevelType, typename T, T value>
class MatrixBase
{
	using Iterator = typename std::map<std::size_t, LevelType>::iterator;
public:
	MatrixBase() {};
	std::size_t size() {
		//invalidate();
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
		if(!contain(std::get<0>(id))) {
			return false;
		}
		if constexpr(sizeof...(T) > 0) {
			return rows[std::get<0>(id)].contain(tuple_tail(id));
		}
		else {
			return true;
		}
	}
	template <typename ...Args>
	typename std::enable_if_t<is_idx_type_v<Args...>, bool> erase(std::tuple<Args...> id) {
		if(auto it = rows.find(idx); it != rows.end()) {
			rows.erase(it);
		}
	}
	void invalidate() {
		/*for (auto it = cells.cbegin(); it != cells.cend();)
		{
			if ((*it).second == -1)
			{
				it = cells.erase(it);
			}
			else
			{
				++it;
			}
		}*/
	}
	
	Iterator begin() { invalidate(); return rows.begin(); }

	Iterator end()   { invalidate(); return rows.end(); }
private:
	std::map<std::size_t, LevelType> rows;
};


template <typename T, T value>
using Row = MatrixBase<T, T, value>;

template <typename T, T value>
using Matrix2D = MatrixBase<Row<T, value>, T, value>;

template <typename T, T value>
class Matrix : Matrix2D<T, value>
{
};
