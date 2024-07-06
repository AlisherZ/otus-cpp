#pragma once

#include <type_traits>
#include <list>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

/// <summary>
/// Type traits. Checks whether value is std::vector.
/// </summary>
template <class T1>
struct is_std_vector { static const bool value = false; };

/// <summary>
/// Type traits. Checks whether value is std::vector.
/// </summary>
template <class T1>
struct is_std_vector<std::vector<T1> > { static const bool value = true; };

/// <summary>
/// Helper variable template. Checks whether value is std::vector.
/// </summary>
template<class T1>
inline constexpr bool is_std_vector_v = is_std_vector<T1>::value;

/// <summary>
/// Type traits. Checks whether value is std::list.
/// </summary>
template <class T1>
struct is_std_list { static const bool value = false; };

/// <summary>
/// Type traits. Checks whether value is std::list.
/// </summary>
template <class T1>
struct is_std_list<std::list<T1> > { static const bool value = true; };

/// <summary>
/// Helper variable template. Checks whether value is std::list.
/// </summary>
template<class T1>
inline constexpr bool is_std_list_v = is_std_list<T1>::value;

/// <summary>
/// Type traits. Checks whether value is std::tuple.
/// </summary>
template <class T1>
struct is_std_tuple { static const bool value = false; };

/// <summary>
/// Type traits. Checks whether value is std::tuple.
/// </summary>
template <class... T1>
struct is_std_tuple<std::tuple<T1 ...> > { static const bool value = true; };

/// <summary>
/// Helper variable template. Checks whether value is std::tuple.
/// </summary>
template<class T1>
inline constexpr bool is_std_tuple_v = is_std_list<T1>::value;

/// <summary>
/// Type traits. Checks whether value is std::tuple with same type of elements.
/// </summary>
template <class T1>
struct is_same_type_tuple { static const bool value = false; };

/// <summary>
/// Type traits. Checks whether value is std::tuple with same type of elements.
/// </summary>
template <>
struct is_same_type_tuple<std::tuple<> > { static const bool value = true; };

/// <summary>
/// Type traits. Checks whether value is std::tuple with same type of elements.
/// </summary>
template <class T1, class... U1>
struct is_same_type_tuple<std::tuple<T1, U1...> > : std::conjunction<std::is_same<T1, U1> ...> {};

/// <summary>
/// Helper variable template. Checks whether value is std::tuple with same type of elements.
/// </summary>
template <class T1>
inline constexpr bool is_same_type_tuple_v = is_same_type_tuple<T1>::value;

/// <summary>
/// Print ip address in console. All parts are separated by '.'.
/// </summary>
/// <param name="ip_address">Integer representation of ip address. Each byte is byte of ip</param>
template <typename T>
typename std::enable_if_t<std::is_integral_v<std::decay_t<T> > > print_ip(T ip_address)
{
    std::size_t num_bytes = sizeof(T);
    constexpr uint8_t extract_byte = (uint8_t)-1;
    std::cout << ((ip_address >> (num_bytes - 1) * 8) & extract_byte);
    for(num_bytes--;num_bytes > 0;num_bytes--) {
        std::cout << "." << ((ip_address >> (num_bytes - 1) * 8) & extract_byte);
    }
    std::cout << std::endl;
}

/// <summary>
/// Print ip address in console. All parts are separated by '.'.
/// </summary>
/// <param name="ip_address">String representation of ip address.</param>
template <typename T>
typename std::enable_if_t<std::is_same_v<std::decay_t<T>, std::string>> print_ip(T ip_address)
{
    std::cout << ip_address << std::endl;
}

/// <summary>
/// Print ip address in console. All parts are separated by '.'.
/// </summary>
/// <param name="ip_address">std container as ip address. Each element is byte of ip.</param>
template <typename T>
typename std::enable_if_t<std::disjunction_v<is_std_vector<std::decay_t<T> >, is_std_list<std::decay_t<T> > > > print_ip(T ip_address)
{
    bool first = true;
    for(int ip : ip_address) {
        if(first) {
            first = false;
        }
        else {
            std::cout << ".";
        }
        std::cout << ip;
    }
    std::cout << std::endl;
}

/// <summary>
/// Print ip address in console. All parts are separated by '.'.
/// </summary>
/// <param name="ip_address">std tuple as ip address. Each element is byte of ip. All elements must be same type</param>
template <typename T>
typename std::enable_if_t<std::conjunction_v<is_std_tuple<std::decay_t<T> >, is_std_list<std::decay_t<T> > > > print_ip(T ip_address)
{
    bool first = true;
    for(int ip : ip_address) {
        if(first) {
            first = false;
        }
        else {
            std::cout << ".";
        }
        std::cout << ip;
    }
    std::cout << std::endl;
}

template<std::size_t I = 0, typename... T>
void for_each_print(std::tuple<T...>& t) {
    if constexpr(I > 0) {
        std::cout << "." << std::get<I>(t);
    }
    else {
        std::cout << std::get<I>(t);
    }
    if constexpr(I + 1 != sizeof...(T)) {
        for_each_print<I + 1>(t);
    }
}

/// <summary>
/// Extract the byte representation of a structure from the memory stream.
/// </summary>
/// <param name="ip_address">The location of the structure in the memory stream.</param>
template <typename T>
typename std::enable_if_t<is_same_type_tuple_v<std::decay_t<T> > > print_ip(T ip_address)
{
    if constexpr(std::tuple_size_v<T> == 0) {
        std::cout << std::endl;
    }
    else {
        for_each_print(ip_address);
        std::cout << std::endl;
    }
}
