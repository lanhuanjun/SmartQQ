#ifndef __PLATFORM_INCLUDE__
#define __PLATFORM_INCLUDE__

#if defined(WIN32) || defined(WIN64) || (_WIN32) || (_WIN64)
#include <windows.h>
#include <string>
#endif

#if defined(WIN32) || defined(WIN64) || (_WIN32) || (_WIN64)
typedef SSIZE_T ssize_t;//curl ÖÐ»á±¨´í
typedef UINT64 uint64;
#else
typedef uint64_t uint64;
#endif

#if defined(WIN32) || defined(WIN64) || (_WIN32) || (_WIN64)

#pragma warning (push)
#pragma warning (disable: 4996)
#include "../../../third/include/json/json.h"
#pragma warning (pop)

#else

#include "../../../third/include/json/json.h"

#endif
std::string get_exe_path();
#endif // __PLATFORM_INCLUDE__