#ifndef __PLATFORM_INCLUDE__
#define __PLATFORM_INCLUDE__

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#include <string>
#endif

#if defined(WIN32) || defined(WIN64)
typedef SSIZE_T ssize_t;//curl ÖÐ»á±¨´í
typedef UINT64 uint64;
#else
typedef uint64_t uint64;
#endif

std::string get_exe_path();
#endif // __PLATFORM_INCLUDE__