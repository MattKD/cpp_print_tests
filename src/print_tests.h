#ifndef IO_TEST_H
#define IO_TEST_H

#include <ctime>

void printUsage();
size_t strToUInt(const char *str);

//
// Templated tests for C, C++, variadic template, and KameUtil-Print
// style formatting
//

static const char *str = "world";
static const char *str2 = "happy";
static int x = 10;
static int y = 20;
static double a = 17;
static double b = 3;

template <typename PrintFunc>
double printfStyleTest(PrintFunc &func, size_t iterations)
{
  clock_t start = clock(); 

  for (size_t i = 0; i < iterations; ++i) {
    func("blah blah blah\n");
    func("hello number %d!\n", x);
    func("%d + %d = %d\n", x, y, x + y);
    func("hello %s! Are you %s today? Pos = (%d, %d)\n", str, str2, x, y);
    func("%d%d%d%d%d%f%f\n", x, y, 3, 2, 1, a * b, a / b);
  }

  func.flush();

  clock_t end = clock(); 
  double duration = 1000.0 * (end-start) / CLOCKS_PER_SEC;
  return duration;
}

template <typename OutStream>
double cppStyleTest(OutStream &os, size_t iterations)
{
  clock_t start = clock(); 

  for (size_t i = 0; i < iterations; ++i) {
    os << "blah blah blah\n";
    os << "hello number " << x << "!\n";
    os << x << " + " << y << " = " << x + y << "\n";
    os << "hello " << str << "! Are you " << str2 << " today? Pos = (" << x 
       << ", " << y << ")\n";
    os << x << y << 3 << 2 << 1 << a * b << a / b << "\n";
  }

  os.flush();

  clock_t end = clock(); 
  double duration = 1000.0 * (end-start) / CLOCKS_PER_SEC;
  return duration;
}

template <typename PrintFunc>
double KameUtilPrintStyleTest(PrintFunc &func, size_t iterations)
{
  clock_t start = clock(); 

  for (size_t i = 0; i < iterations; ++i) {
    func("blah blah blah\n");
    func("hello number {}!\n", x);
    func("{} + {} = {}\n", x, y, x + y);
    func("hello {}! Are you {} today? Pos = ({}, {})\n", str, str2, x, y);
    func("{}{}{}{}{}{}{}\n", x, y, 3, 2, 1, a * b, a / b);
  }

  func.flush();

  clock_t end = clock(); 
  double duration = 1000.0 * (end-start) / CLOCKS_PER_SEC;
  return duration;
}

typedef double (*TestFunc)(size_t iterations);
typedef double (*FileTestFunc)(size_t iterations, char *buff, size_t buff_size);

struct BaseTest {
  BaseTest(size_t iterations, const char *test_name) 
    : times_run{0}, iterations{iterations}, test_name{test_name}, 
      duration{0.0f} { }

  virtual ~BaseTest() { }
  virtual double runImpl(size_t iters) = 0;

  void run() { duration += runImpl(iterations); ++times_run; }
  double average() { return times_run == 0 ? 0 : duration / times_run; }

  size_t times_run;
  size_t iterations;
  const char *test_name;
  double duration;
};


struct Test : BaseTest {
  Test(TestFunc func, size_t iters, const char *test_name)
    : BaseTest(iters, test_name), test{func} { }

  double runImpl(size_t iters) override
  { 
    return test(iterations); 
  }

  TestFunc test;
};

struct FileTest : BaseTest {
  FileTest(FileTestFunc func, size_t iters, const char *test_name, 
    char *buff = nullptr, size_t buff_size = 0)
    : BaseTest(iters, test_name), buff{buff}, buff_size{buff_size}, 
      file_test{func} { }

  double runImpl(size_t iters) override
  { 
    return file_test(iterations, buff, buff_size); 
  }

  char *buff;
  size_t buff_size;
  FileTestFunc file_test;
};


#endif
