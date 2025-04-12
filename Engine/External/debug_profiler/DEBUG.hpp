

#include <Global.hpp>
#include <sstream>
#include <string>
#include <tracy/Tracy.hpp>
// config
#define DEBUG_PROFILE 1
#define PROFILE_ONLY 0
#define LOG_ONLY 1

//

#pragma once
#if DEBUG_PROFILE
#if defined(__clang__) || defined(__GNUC__)
#define FUNCTION_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define FUNCTION_NAME __FUNCSIG__
#endif

#include <chrono>
#include <cstdint>
#include <fstream>
#include <string_view>
// coroutine for file and console  logging

// TODO: ADD SUPPORT FOR FUNCTION BENCHMARKING
//  struct function_timer {
//
//    function_timer();
//
//    ~function_timer();
//  };
//  class to maintain debuging in debug builds with tracy profiling

// #if defined(__clang__) || defined(__GNUC__)
// #define TYPE_OF abi::__cxa_demangle
// #elif defined(_MSC_VER)
#define TYPE_OF(x) typeid(x).name()
// #endif

class Debug_profiler
{
    std::fstream     logout_txt;
    int64_t          min, sec, milli, micro, nano;
    std::string_view str;
    std::chrono::time_point<std::chrono::high_resolution_clock,
                            std::chrono::nanoseconds>
         start_Time, func_start, func_end;
    char file_buff[10248]{};
    int  lastindex = 0;

    void get_passed_time();
    void set_time(std::chrono::nanoseconds fraction);

   public:
    Debug_profiler();
    ~Debug_profiler();
    void log(std::string_view file,
             int              line,
             const char      *func,
             std::string      type_info);
    void log(std::string_view file, int line, const char *func);

    void print_to_console(std::string_view time,
                          std::string_view file,
                          int              line,
                          std::string_view func,
                          std::string_view type_info);

    void console_log(int         Line,
                     const char *function,
                     const char *file,
                     std::string log);

    // ~Debug_profiler();
};
inline Debug_profiler LOGGER;
template <class... Args>
inline constexpr std::string type_arg(Args... args)

{
    std::ostringstream ss;
    ((ss << args << "\t"), ...);
    return ss.str();
}

#if PROFILE_ONLY
#define FUNCTION_PROFILE ZoneScopedN(FUNCTION_NAME);
#define FUNCTION_PROFILE_ARG(...) ZoneScopedN(FUNCTION_NAME);

#endif

#if !LOG_ONLY
#ifndef FUNCTION_PROFILE_ARG(...)

#define FUNCTION_PROFILE_ARG(...)                                         \
    LOGGER.log(__FILE__, __LINE__, FUNCTION_NAME, type_arg(__VA_ARGS__)); \
    ZoneScopedN(FUNCTION_NAME);
#endif  // FUNCTION_PROFILE_ARG(...)

#ifndef FUNCTION_PROFILE

#define FUNCTION_PROFILE                           \
    LOGGER.log(__FILE__, __LINE__, FUNCTION_NAME); \
    ZoneScopedN(FUNCTION_NAME);

#endif  // FUNCTION_PROFILE
#else

#define FUNCTION_PROFILE

#define FUNCTION_PROFILE_ARG(...)

#endif
#ifndef LOG
#define LOG(...)        \
    LOGGER.console_log( \
        __LINE__, FUNCTION_NAME, __FILE__, type_arg(__VA_ARGS__));
#endif  // LOG

#define END_LOG LOGGER.~Debug_profiler();
#else

#define TRACER

#define FUNCTION_PROFILE

#define FUNCTION_PROFILE_ARG(...)

#define LOG(...)

#define END_LOG
#endif  // DEBU_PROFILE
