#ifndef KAME_UTIL_FWD_H
#define KAME_UTIL_FWD_H

#include "KameUtil/stream_print.h"

template <class OStream>
struct KameUtilPrintFwd {
  KameUtilPrintFwd(OStream &&os) : os{std::move(os)} { }

  template <class ...Args>
  void operator()(Args &&...args)
  {
    KameUtil::streamPrint(os, std::forward<Args>(args)...);
  }

  void flush() { os.flush(); }

  OStream os;
};

#endif
