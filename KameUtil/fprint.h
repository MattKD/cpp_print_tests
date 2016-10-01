#ifndef KAME_UTIL_FPRINT_H
#define KAME_UTIL_FPRINT_H

#include "stream_print.h"
#include <fstream>

namespace KameUtil {

template <class ...Args>
bool fprint(std::ofstream &fout, const char *fmt, const Args &...args)
{
  return streamPrint<std::ostream>(fout, fmt, args...);
}

template <class ...Args>
bool wfprint(std::wofstream &fout, const wchar_t *fmt, const Args &...args)
{
  return streamPrint<std::wostream>(fout, fmt, args...);
}


} // end namespace KameUtil

#endif
