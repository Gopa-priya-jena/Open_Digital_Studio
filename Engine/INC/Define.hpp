#define DEBUG 1

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define ODS_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
#define ODS_LINUX 1
#if defined(__ANDROID__)
#define ODS_ANDROID 1
#endif
#elif __APPLE__
#define ODS_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#define ODS_IOS 1
#define ODS_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define ODS_IOS 1
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform!"
#endif
// now  checking in cmpiler for intrinsics

#if defined(__GNUG__)
#define GCC 1
#define CLANG 0
#define MSVC 0
#elif defined(__clang__)
#define CLANG 1
#define GCC 0
#define MSVC 0
#elif defined(_MSC_VER)
#define MSVC 1
#define CLANG 0
#define GCC 0
#endif
