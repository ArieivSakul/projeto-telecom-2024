#pragma once

#include <vector>
#include <inttypes.h>
#include <exception>
#include <stdexcept>
#include <map>

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#if defined(__MINGW64__)
#if LONG == __LONG32
#ifdef LONG
#undef LONG
#endif
#define LONG long long
#endif
#endif

#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#include <sal.h>
#ifndef _Notnull_
#define _Notnull_
#endif
#endif
