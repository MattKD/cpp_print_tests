#include "vtemplate_print.h"
#include "print_tests.h"
#include "kame_util_fwd.h"
#include <cstring>
#include <sstream>
#include <string>

namespace {

// For testing snprintf using a regular array with printfStyleTest.
// This is simulating the case where the user knows the buffer is 
// big enough or doesn't care about truncation.
struct SprintfFwd {
  SprintfFwd() : pos{0} { }

  template <typename... Args>
  void operator()(Args&& ...args)
  {
    // reset to 0 when almost full for testing, since tests may be called
    // 1000's of times.
    if (buff_size - pos < 128) {
      pos = 0;
    }
    int n = snprintf(buff+pos, buff_size, std::forward<Args>(args)...);
    if (n > 0) {
      pos += n;
    }
  }

  void flush() { } // need flush for tests

  static const int buff_size = 4096;
  char buff[buff_size];
  int pos;
};

// For testing snprintf and std::string append functions with the
// variadic template and KameUtil-Print functions.
struct SprintfStream {
  SprintfStream& operator<<(char c) 
  { 
    str.push_back(c); 
    return *this; 
  }

  SprintfStream& operator<<(const char *s) 
  { 
    str.append(s); 
    return *this;
  }

  SprintfStream& operator<<(int n)
  {
    const int buff_size = 64;
    char int_str[buff_size];
    size_t int_str_len = snprintf(int_str, buff_size, "%d", n);
    if (int_str_len > 0) {
      str.append(int_str, int_str_len);
    }
    return *this;
  }

  SprintfStream& operator<<(double d)
  {
    const int buff_size = 2048;
    char dbl_str[buff_size];
    size_t dbl_str_len = snprintf(dbl_str, buff_size, "%f", d);
    if (dbl_str_len > 0) {
      str.append(dbl_str, dbl_str_len);
    }
    return *this;
  }

  void flush() { } // need flush for tests

  std::string str;
};

} // end anon namespace

// Tests snprintf
double snprintfTest(size_t iterations)
{
  SprintfFwd func;
  return printfStyleTest(func, iterations);
}

// Tests std::ostringstream
double stringstreamTest(size_t iterations)
{
  std::stringstream ss;
  return cppStyleTest(ss, iterations);
}

// variadic template using string and snprintf
double vtemplate_snprintfTest(size_t iterations)
{
  auto func = VTemplatePrintFwd<SprintfStream>(SprintfStream());
  return printfStyleTest(func, iterations);
}

// variadic template using stringstream
double vtemplate_sstreamTest(size_t iterations)
{
  auto func = VTemplatePrintFwd<std::stringstream>(std::stringstream());
  return printfStyleTest(func, iterations);
}

// Tests KameUtil::streamPrint using stringstream
double KameUtil_sstreamTest(size_t iterations)
{
  auto func = KameUtilPrintFwd<std::stringstream>(std::stringstream()); 
  return KameUtilPrintStyleTest(func, iterations);
}

// Tests KameUtil::streamPrint using snprintf
double KameUtil_snprintfTest(size_t iterations)
{
  auto func = KameUtilPrintFwd<SprintfStream>(SprintfStream());
  return KameUtilPrintStyleTest(func, iterations);
}
