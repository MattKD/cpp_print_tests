#ifndef KAME_UTIL_SPRINT_H
#define KAME_UTIL_SPRINT_H

#include "stream_print.h"
#include <sstream>

namespace KameUtil {

template <class ...Args>
bool sprint(std::stringstream &ss, const char *fmt, const Args &...args)
{
  return streamPrint<std::ostream>(ss, fmt, args...);
}

template <class ...Args>
bool wsprint(std::wstringstream &ss, const wchar_t *fmt, const Args &...args)
{
  return streamPrint<std::wostream>(ss, fmt, args...);
}


} // end namespace KameUtil

#endif