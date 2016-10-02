#ifndef KAME_UTIL_PRINT_H
#define KAME_UTIL_PRINT_H

#include "stream_print.h"
#include <iostream>

namespace KameUtil {

template <class ...Args>
bool print(std::ostream &os, const char *fmt, const Args &...args)
{
  return streamPrint(os, fmt, args...);
}

template <class ...Args>
bool print(const char *fmt, const Args &...args)
{
  return print(std::cout, fmt, args...);
}

template <class ...Args>
bool print(std::wostream &os, const wchar_t *fmt, const Args &...args)
{
  return streamPrint(os, fmt, args...);
}

template <class ...Args>
bool print(const wchar_t *fmt, const Args &...args)
{
  return print(std::wcout, fmt, args...);
}

} // end namespace KameUtil

#endif
