//          Matthew Avery Coder 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef UTIL_STATIC_LOGGING
#define UTIL_STATIC_LOGGING

#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>

// from http://stackoverflow.com/questions/7724448/simple-json-string-escape-for-c
namespace {
inline std::string escape_json_string(const char * input) {
    std::ostringstream ss;
    while (*input != '\0') {
        switch (*input) {
            case '\\': ss << "\\\\"; break;
            case '"': ss << "\\\""; break;
            case '/': ss << "\\/"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default: ss << *input; break;
        }
        ++input;
    }
    return ss.str();
}
}

static std::mutex logging_mutex;

#ifdef LOG_LEVEL_TRACE
#define LOG_TRACE(_x) {\
  uint64_t __log_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();\
  std::ostringstream _log_format_oss;\
  _log_format_oss << _x;\
  std::lock_guard<std::mutex> lock(logging_mutex);\
  std::cout << "{\"time\":\""<<  __log_time\
  << "\", \"thread\":\"" << std::this_thread::get_id() \
  << "\",\"level\":\"TRACE\", \"file\":\"" \
  << __FILE__ << "\", \"function\":\"" \
  << __func__ << "\", \"line\":" \
  << __LINE__ \
  << ", \"data\": \"" << escape_json_string(_log_format_oss.str().c_str()) << "\"}\n";}
#define LOG_TRACE_RUN(x) x;
#define LOG_LEVEL_DEBUG
#else
#define LOG_TRACE(x) ;
#define LOG_TRACE_RUN(x) ;
#endif

#ifdef LOG_LEVEL_DEBUG
#define LOG_DEBUG(_x) {\
  uint64_t __log_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();\
  std::ostringstream _log_format_oss;\
  _log_format_oss << _x;\
  std::lock_guard<std::mutex> lock(logging_mutex);\
  std::cout << "{\"time\":\""<<  __log_time\
  << "\", \"thread\":\"" << std::this_thread::get_id() \
  << "\",\"level\":\"DEBUG\", \"file\":\"" \
  << __FILE__ << "\", \"function\":\"" \
  << __func__ << "\", \"line\":" \
  << __LINE__ \
  << ", \"data\": \"" << escape_json_string(_log_format_oss.str().c_str()) << "\"}\n";}
#define LOG_DEBUG_RUN(x) x;
#define LOG_LEVEL_INFO
#else
#define LOG_DEBUG(x) ;
#define LOG_DEBUG_RUN(x) ;
#endif

#ifdef LOG_LEVEL_INFO
#define LOG_INFO(_x) {\
  uint64_t __log_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();\
  std::ostringstream _log_format_oss;\
  _log_format_oss << _x;\
  std::lock_guard<std::mutex> lock(logging_mutex);\
  std::cout << "{\"time\":\""<<  __log_time\
  << "\", \"thread\":\"" << std::this_thread::get_id() \
  << "\",\"level\":\"INFO\", \"file\":\"" \
  << __FILE__ << "\", \"function\":\"" \
  << __func__ << "\", \"line\":" \
  << __LINE__ \
  << ", \"data\": \"" << escape_json_string(_log_format_oss.str().c_str()) << "\"}\n";}
#define LOG_INFO_RUN(x) x;
#define LOG_LEVEL_WARNING
#else
#define LOG_INFO(x) ;
#define LOG_INFO_RUN(x) x;
#endif

#ifdef LOG_LEVEL_WARNING
#define LOG_WARNING(_x) {\
  uint64_t __log_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();\
  std::ostringstream _log_format_oss;\
  _log_format_oss << _x;\
  std::lock_guard<std::mutex> lock(logging_mutex);\
  std::cout << "{\"time\":\""<<  __log_time\
  << "\", \"thread\":\"" << std::this_thread::get_id() \
  << "\",\"level\":\"WARNING\", \"file\":\"" \
  << __FILE__ << "\", \"function\":\"" \
  << __func__ << "\", \"line\":" \
  << __LINE__ \
  << ", \"data\": \"" << escape_json_string(_log_format_oss.str().c_str()) << "\"}\n";}
#define LOG_WARNING_RUN(x) x;
#define LOG_LEVEL_ERROR
#else
#define LOG_WARNING(x) ;
#define LOG_WARNING_RUN(x) ;
#endif

#ifdef LOG_LEVEL_ERROR
#define LOG_ERROR(_x) {\
  uint64_t __log_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();\
  std::ostringstream _log_format_oss;\
  _log_format_oss << _x;\
  std::lock_guard<std::mutex> lock(logging_mutex);\
  std::cout << "{\"time\":\""<<  __log_time\
  << "\", \"thread\":\"" << std::this_thread::get_id() \
  << "\",\"level\":\"ERROR\", \"file\":\"" \
  << __FILE__ << "\", \"function\":\"" \
  << __func__ << "\", \"line\":" \
  << __LINE__ \
  << ", \"data\": \"" << escape_json_string(_log_format_oss.str().c_str()) << "\"}\n";}
#define LOG_ERROR_RUN(x) x;
#define LOG_LEVEL_FATAL
#else
#define LOG_ERROR(x) ;
#define LOG_ERROR_RUN(x) ;
#endif

#ifdef LOG_LEVEL_FATAL
#define LOG_FATAL(_x) {\
  uint64_t __log_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();\
  std::ostringstream _log_format_oss;\
  _log_format_oss << _x;\
  std::lock_guard<std::mutex> lock(logging_mutex);\
  std::cout << "{\"time\":\""<<  __log_time\
  << "\" , \"thread\":\"" << std::this_thread::get_id() \
  << "\",\"level\":\"FATAL\", \"file\":\"" \
  << __FILE__ << "\", \"function\":\"" \
  << __func__ << "\", \"line\":" \
  << __LINE__ \
  << ", \"data\": \"" << escape_json_string(_log_format_oss.str().c_str()) << "\"}\n";}
#define LOG_FATAL_RUN(x) x;
#else
#define LOG_FATAL(x);
#define LOG_FATAL_RUN(x) ;
#endif

#endif
