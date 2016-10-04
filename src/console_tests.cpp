#include "print_tests.h"
#include "vtemplate_print.h"
#include "kame_util_fwd.h"
#include <cstdio>
#include <iostream>

namespace {

// For testing printf with template test functions
struct PrintfFwd {
  template <class ...Args>
  void operator()(Args &&...args)
  {
    printf(std::forward<Args>(args)...);
  }

  void flush() { fflush(stdout); }
};

// Stream object using cstdio stdout functions
struct CSTDIO_Stream {
  CSTDIO_Stream& operator<<(char c) { putc(c, stdout); return *this; }
  CSTDIO_Stream& operator<<(int i) { printf("%d", i); return *this; }
  CSTDIO_Stream& operator<<(double d) { printf("%f", d); return *this; }
  CSTDIO_Stream& operator<<(const char *s) 
  { 
    fputs(s, stdout); 
    return *this; 
  }

  void flush() { fflush(stdout); }
};

// To use cout with variadic template print and KameUtil-Print
struct CoutFwd {
  CoutFwd& operator<<(char c) { std::cout << c; return *this; }
  CoutFwd& operator<<(int n) { std::cout << n; return *this; }
  CoutFwd& operator<<(double d) { std::cout << d; return *this; }
  CoutFwd& operator<<(const char *s) { std::cout << s; return *this; }

  template <typename T>
  CoutFwd& operator<<(const T &t) { std::cout << t; return *this; }

  void flush() { std::cout.flush(); }
};

} // end anon namespace

// Tests printf
double printfTest(size_t iterations)
{
  PrintfFwd func;
  return printfStyleTest(func, iterations);
}

// Tests cout
double coutTest(size_t iterations)
{
  return cppStyleTest(std::cout, iterations);
}

// Tests variadic template print using printf
double vtemplate_printfTest(size_t iterations)
{
  auto func = VTemplatePrintFwd<CSTDIO_Stream>(CSTDIO_Stream());
  return printfStyleTest(func, iterations);
}

// Tests variadic template print using cout
double vtemplate_coutTest(size_t iterations)
{
  auto func = VTemplatePrintFwd<CoutFwd>(CoutFwd());
  return printfStyleTest(func, iterations);
}

// KameUtil-Print using printf
double KameUtil_printfTest(size_t iterations)
{
  auto func = KameUtilPrintFwd<CSTDIO_Stream>(CSTDIO_Stream());
  return printfStyleTest(func, iterations);
}

// KameUtil-Print using cout
double KameUtil_coutTest(size_t iterations)
{
  auto func = KameUtilPrintFwd<CoutFwd>(CoutFwd());
  return KameUtilPrintStyleTest(func, iterations);
}
