#include "print_tests.h"
#include "template_fprintf.h"
#include "KameUtil/print.h"
#include <cstdio>
#include <iostream>

double printfTest(size_t iterations)
{
  return printfStyleTest(printf, iterations);
}

double coutTest(size_t iterations)
{
  return cppStyleTest(std::cout, iterations);
}

namespace {

// Stream object to test the templated FPrintf with stdio functions 
// to print to stdout
struct StdoutStream {
  StdoutStream& operator<<(char c) { putc(c, stdout); return *this; }
  StdoutStream& operator<<(int i) { printf("%d", i); return *this; }
  StdoutStream& operator<<(double d) { printf("%f", d); return *this; }
  StdoutStream& operator<<(const char *s) { fputs(s, stdout); return *this; }
};

}

double templatePrintfTest(size_t iterations)
{
  FPrintfFunctor<StdoutStream> os{StdoutStream{}};
  return printfStyleTest(os, iterations);
}

namespace {

struct CoutFwd {
  template <typename T>
  CoutFwd& operator<<(T &t) { std::cout << t; return *this; }
};

}

double templateCppPrintfTest(size_t iterations)
{
  FPrintfFunctor<CoutFwd> os(CoutFwd{});
  return printfStyleTest(os, iterations);
}

namespace {

struct KameUtilPrintFwd {
  template <class ...Args>
  void operator()(Args &&...args)
  {
    KameUtil::print(std::forward<Args>(args)...);
  }
};

}

double KameUtilPrintTest(size_t iterations)
{
  KameUtilPrintFwd func;
  return KameUtilPrintStyleTest(func, iterations);
}




