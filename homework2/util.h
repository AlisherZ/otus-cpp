#pragma once

template <typename T, typename... Args>
auto filter(T ip_pool, Args... value) -> decltype(ip_pool);

template <typename T>
auto filter_any(T ip_pool, int value) -> decltype(ip_pool);

template <typename T>
void print_pool(T ip_pool);

template <typename T>
bool comp_eq_pool(T ip_pool1, T ip_pool2);
