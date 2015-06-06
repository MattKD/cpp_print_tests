#ifndef IO_TEST_H
#define IO_TEST_H

#include <ctime>

void printUsage();
size_t strToInt(const char *str);

//
// Templated tests for c and c++ style printing
//

template <typename PrintFunc>
double printfStyleTest(PrintFunc &func, size_t iterations)
{
  const char *str = "world";
  const char *str2 = "happy";
  int x = 10, y = 20;
  double a = 17, b = 3;
  clock_t start = clock(); 

  for (size_t i = 0; i < iterations; ++i) {
    func("blah blah blah\n");
    func("hello number %d!\n", x);
    func("%d + %d = %d\n", x, y, x + y);
    func("hello %s! Are you %s today? Pos = (%d, %d)\n", str, str2, x, y);
    func("%d%d%d%d%d%f%f\n", x, y, 3, 2, 1, a * b, a / b);
  }

  clock_t end = clock(); 
  double duration = 1000.0 * (end-start) / CLOCKS_PER_SEC;
  return duration;
}

template <typename OutStream>
double cppStyleTest(OutStream &os, size_t iterations)
{
  const char *str = "world";
  const char *str2 = "happy";
  int x = 10, y = 20;
  double a = 17, b = 3;
  clock_t start = clock(); 

  for (size_t i = 0; i < iterations; ++i) {
    os << "blah blah blah\n";
    os << "hello number " << x << "!\n";
    os << x << " + " << y << " = " << x + y << "\n";
    os << "hello " << str << "! Are you " << str2 << " today? Pos = (" << x 
       << ", " << y << ")\n";
    os << x << y << 3 << 2 << 1 << a * b << a / b << "\n";
  }

  clock_t end = clock(); 
  double duration = 1000.0 * (end-start) / CLOCKS_PER_SEC;
  return duration;
}

typedef double (*TestFunc)(size_t iterations);
typedef double (*FileTestFunc)(size_t iterations, char *buff, size_t buff_size);

template <typename TestFunction>
struct BaseTest {
  BaseTest(TestFunction test, size_t iterations, const char *test_name) 
    : times_run{0}, iterations{iterations}, test_name{test_name}, 
      duration{0.0f}, test{test} { }

  void run() { duration += test(iterations); ++times_run; }
  double average() { return times_run == 0 ? 0 : duration / times_run; }

  size_t times_run;
  size_t iterations;
  const char *test_name;
  double duration;
  TestFunction test;
};

struct FileTestFunctor {
  FileTestFunctor(FileTestFunc test) 
    : FileTestFunctor(test, nullptr, 0) { }
  FileTestFunctor(FileTestFunc test, char *buff, size_t buff_size)
    : buff{buff}, buff_size{buff_size}, file_test{test} { }

  double operator()(size_t iterations) 
  { 
    return file_test(iterations, buff, buff_size); 
  }

  char *buff;
  size_t buff_size;
  FileTestFunc file_test;
};

typedef BaseTest<TestFunc> Test;
typedef BaseTest<FileTestFunctor> FileTest;

#endif
