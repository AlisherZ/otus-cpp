#pragma once

#include <type_traits>
#include <list>
#include <iostream>
#include <string>
#include <vector>

template <class T1>
struct is_std_vector { static const bool value=false; };

template <class T1>
struct is_std_vector<std::vector<T1> > { static const bool value=true; };

template <class T1>
struct is_std_list { static const bool value=false; };

template <class T1>
struct is_std_list<std::list<T1> > { static const bool value=true; };

template <typename T>
typename std::enable_if_t<std::is_integral<std::decay_t<T> >::value> print_ip(T ip_address)
{
    std::size_t num_bytes = sizeof(T);
    constexpr uint8_t extract_byte = (uint8_t)-1;
    std::cout << ((ip_address >> (num_bytes - 1) * 8) & extract_byte);
    for(num_bytes--;num_bytes > 0;num_bytes--) {
        std::cout << "." << ((ip_address >> (num_bytes - 1) * 8) & extract_byte);
    }
    std::cout << std::endl;
}

template <typename T>
typename std::enable_if_t<std::is_same<std::decay_t<T>, std::string>::value> print_ip(T ip_address)
{
    std::cout << ip_address << std::endl;
}

template <typename T>
typename std::enable_if_t<std::disjunction<is_std_vector<std::decay_t<T> >, is_std_list<std::decay_t<T> > >::value> print_ip(T ip_address)
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
