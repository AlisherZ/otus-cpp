#include "util.h"
#include <vector>
#include <string>

template <typename T>
auto filter_any(T ip_pool, int value) -> decltype(ip_pool) {
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
