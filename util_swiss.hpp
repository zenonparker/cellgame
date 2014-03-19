//          Matthew Avery Coder 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef UTIL_SWISS
#define UTIL_SWISS

//various random _small_ functions

#include <string>
#include <sstream>
#include <ios>
#include <cstdio>
#include <iterator>
#include <limits>
#include <cpp11_polyfill.hpp>

namespace util {
namespace Swiss {

inline char *strnstr(const char *s, const char *find, const size_t len) { 
  const size_t find_len = strlen(find);
  for (size_t i = 0; i <= len - find_len; ++i) {
    if (strncmp(s + i, find, find_len) == 0) {
      return (char *) s + i;
    }
  }
  return 0;
}

template <class T>
bool is_primitive(const std::string &str) {
  std::istringstream iss(str);
  T t;
  iss >> std::noskipws >> t;
  return iss.eof() && !iss.fail();
}

template <class T>
T* throw_null(T* ptr) {
  if(ptr == nullptr) { throw std::runtime_error("Null pointer"); }
  else { return ptr; }
}

CONSTEXPR_PF char* to_char(uint8_t *ptr) {
  return reinterpret_cast<char*>(ptr);
}

CONSTEXPR_CONST_PF char* to_char(const uint8_t *ptr) {
  return reinterpret_cast<const char*>(ptr);
}

CONSTEXPR_PF uint8_t* to_byte(char * ptr) {
  return reinterpret_cast<uint8_t *>(ptr);
}

CONSTEXPR_CONST_PF uint8_t* to_byte(const char * ptr) {
  return reinterpret_cast<const uint8_t *>(ptr);
}

template<class T>
CONSTEXPR_PF int max_bytes_required() {
  //+1 for the sign, +1 for the extra digit
  //since digits10 will return 9 for an int
  //because it can't represent the full range
  //of 10 digit numbers, even though some of 
  //the numbers are 10 gits long (like 2^30)
  //
  //if the number is a float or double, a we calculate
  //the number of bytes differently (stolen from std::string<T>::to_string)
  return std::numeric_limits<T>::is_integer ? std::numeric_limits<T>::digits10 + 2 :     
    std::numeric_limits<T>::max_exponent10 + 20;
}

//TODO with templates
CONSTEXPR_CONST_PF char *print_format(const int &t) { return "%d"; }
CONSTEXPR_CONST_PF char *print_format(const unsigned int &t) { return "%u"; }
CONSTEXPR_CONST_PF char *print_format(const long &t) { return "%ld"; }
CONSTEXPR_CONST_PF char *print_format(const unsigned long &t) { return "%lu"; }
CONSTEXPR_CONST_PF char *print_format(const long long &t) { return "%lld"; }
CONSTEXPR_CONST_PF char *print_format(const unsigned long long &t) { return "%llu"; }
CONSTEXPR_CONST_PF char *print_format(const float &t) { return "%g"; }
CONSTEXPR_CONST_PF char *print_format(const double &t) { return "%g"; }
CONSTEXPR_CONST_PF char *print_format(const char *t) { return "%s"; }

//returns whether to_buffer succeeded or not. 
//if it did not succeed, num_bytes contains the minium number of bytes needed for
//a successful write.
//If it did succeed, it contains the number of bytes that was written 
//if negative, the number is not parsable

template<class T, class U, class S>
inline bool to_buffer(T * begin, T * end, const U &num, S &num_bytes) {
  S distance = std::distance(begin,end);
  num_bytes = snprintf(reinterpret_cast<char *>(begin), distance,print_format(num), num);
  if(num_bytes < distance) {
    return true;
  } else {
    //add the extra byte needed for the null terminator
    ++num_bytes;
    return false;
  }
}

template<class T, class S>
inline bool to_buffer(T * begin, T * end, const nullptr_t &nptr, S &num_bytes) {
  S distance = std::distance(begin,end);
  num_bytes = 4;
  if(distance >= 4) {
    begin[0] = 'n'; begin[1] = 'u'; begin[2] = 'l'; begin[3] = 'l';
    return true;
  }
  return false;
}

/**
 * @brief calculate the length of a string at compile-time
 * from http://stackoverflow.com/questions/19072539/profiling-constexpr-with-const-char-array-parameters-shows-run-time-execution
 * @param str string to calculate the length of
 * @param index offset to start at from the provided string
 *
 * @return length of null-terminated string
 */
CONSTEXPR_PF std::size_t string_length( const char* str, std::size_t index = 0 ) {
      return ( str == nullptr || str[index] == '\0' ) ? 0 : 1 + string_length( str, index+1 );
}

}
}
#endif
