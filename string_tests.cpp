#include "string.h"
#include "template_fprintf.h"
#include "print_tests.h"
#include <cstring>
#include <sstream>

//
// String tests using a non-resizable array
//

namespace {

// Functor to pass sprintf to printfStyleTest
// using a static buffer. Buffer should be big enough
// for one call in printfSyleTest
struct SprintfFunctor {
  static const int buff_size = 1000;
  char buff[buff_size];
  template <typename... Args>
  void operator()(Args&& ...args)
  {
    sprintf(buff, std::forward<Args>(args)...);
  }
};

}

double sprintfTest(size_t iterations)
{
  SprintfFunctor sf;
  return printfStyleTest(sf, iterations);
}

namespace {

// Stream object used by templateSprintfTest for testing the 
// templated FPrintf with sprintf with a static buffer
struct OutStringStream {
  static const size_t buff_size = 1000;
  size_t size;
  char buff[buff_size];

  OutStringStream() : size{0} {}
  void checkSize(int needed) 
  { 
    if (size + needed >= buff_size - 1) {
      size = 0; 
    }
  }
  OutStringStream& operator<<(char c) 
  { 
    buff[size++] = c; 
    checkSize(0); 

    return *this; 
  }
  OutStringStream& operator<<(int i) 
  { 
    const int tmp_buff_size = 100;
    char tmp_buff[tmp_buff_size];
    int n = sprintf(tmp_buff, "%d", i); 
    checkSize(n);
    strcpy(buff + size, tmp_buff);
    size += n;
    return *this; 
  }
  OutStringStream& operator<<(double d) 
  { 
    const int tmp_buff_size = 500;
    char tmp_buff[tmp_buff_size];
    int n = sprintf(tmp_buff, "%f", d); 
    checkSize(n);
    strcpy(buff + size, tmp_buff);
    size += n;
    return *this; 
  }
  OutStringStream& operator<<(const char *s) 
  { 
    while (*s) { 
      operator<<(*s++);
    }
    return *this; 
  }
};

}

// Tests FPrintf using the sprintf style stream object OutStringStream
double templateSprintfTest(size_t iterations)
{
  FPrintfFunctor<OutStringStream> fpf{OutStringStream{}};
  return printfStyleTest(fpf, iterations);
}

//
// String tests with resizable strings using the heap
//

namespace {

// Functor to pass String to printfStyleTest
struct StringSprintfFunctor {
  String str;

  template <typename... Args>
  void operator()(Args&& ...args)
  {
    str.sprintf(std::forward<Args>(args)...);
  }
};

}

// Tests String::sprintf
double heapSprintfTest(size_t iterations)
{
  StringSprintfFunctor ssf;
  return printfStyleTest(ssf, iterations);
}

// Tests std::ostringstream
double ostringstreamTest(size_t iterations)
{
  std::ostringstream ss;
  return cppStyleTest(ss, iterations);
}

namespace {

// Stream object used by templateHeapSprintfTest for testing the 
// templated FPrintf with String::sprintf
struct OutHeapStringStream {
  String str;

  template <typename T>
  OutHeapStringStream& operator<<(T val) 
  { 
    str.append(val);
    return *this; 
  }
};

}

// Tests template FPrintf using String::sprintf with the stream 
// object OutHeapStringStream
double templateHeapSprintfTest(size_t iterations)
{
  FPrintfFunctor<OutHeapStringStream> fpf{OutHeapStringStream{}};
  return printfStyleTest(fpf, iterations);
}

