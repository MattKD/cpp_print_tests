#ifndef KAME_UTIL_PRINT_H
#define KAME_UTIL_PRINT_H

#include "stream_print.h"
#include <iostream>

namespace KameUtil {

template <class ...Args>
bool print(const char *fmt, const Args &...args)
{
  return streamPrint<std::ostream>(std::cout, fmt, args...);
}

template <class ...Args>
bool wprint(const wchar_t *fmt, const Args &...args)
{
  return streamPrint<std::wostream>(std::wcout, fmt, args...);
}


} // end namespace KameUtil

#endif
