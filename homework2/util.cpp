#include "util.h"

template <typename T, typename... Args>
auto filter(T ip_pool, Args... value) -> decltype(ip_pool){
	std::vector<int> list = {value...};
	auto check = [list](auto line) {
		if(line.size() < list.size()) {
			return false;
		}
		for(size_t i = 0;i < list.size();i++) {
			if(list[i] != stoi(line[i])) {
				return false;
			}
		}
		return true;
	};
	T new_ip_pool;
	for(auto ip : ip_pool) {
		if(check(ip)) {
			new_ip_pool.push_back(ip);
		}
	}
	return new_ip_pool;
}

template <typename T>
auto filter_any(T ip_pool, int value) -> decltype(ip_pool){
	auto check = [value](auto line) {
		for(auto i : line) {
			if(stoi(i) == value) {
				return true;
			}
		}
		return false;
	};
	T new_ip_pool;
	for(auto ip : ip_pool) {
		if(check(ip)) {
			new_ip_pool.push_back(ip);
		}
	}
	return new_ip_pool;
}

template <typename T>
void print_pool(T ip_pool) {
	for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
	{
		for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
		{
			if (ip_part != ip->cbegin())
			{
				std::cout << ".";

			}
			std::cout << *ip_part;
		}
		std::cout << std::endl;
	}
}
