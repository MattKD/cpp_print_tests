#include <cstring>
#include <iostream>
#include <thread>
#include <chrono>
#include "print_tests.h"
#include "console_tests.h"
#include "file_tests.h"
#include "string_tests.h"
#include "KameUtil/print.h"

//
// buffers for console and files using setvbuf and pubsetbuf functions
//
const size_t console_buff_size = 1 << 16; //64Kb
const size_t file_buff_size = console_buff_size;
char console_buff[console_buff_size];
char file_buff[file_buff_size];

int main(int argc, char *argv[])
{  
  using KameUtil::print;
  std::ios::sync_with_stdio(false);
  bool use_setvbuf = true;
  size_t test_iters = 1;
  size_t console_test_iters = 5000;
  size_t file_test_iters = 1000000;
  size_t string_test_iters = 500000;

  try {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "--help") == 0) {
        printUsage();
        return EXIT_SUCCESS;
      } else if (strcmp(argv[i], "-no-setvbuf") == 0) {
        use_setvbuf = false;
      } else if (strcmp(argv[i], "-console") == 0 && i+1 < argc) {
        console_test_iters = strToUInt(argv[++i]);
      } else if (strcmp(argv[i], "-string") == 0 && i+1 < argc) {
        string_test_iters = strToUInt(argv[++i]);
      } else if (strcmp(argv[i], "-file") == 0 && i+1 < argc) {
        file_test_iters = strToUInt(argv[++i]);
      } else if (strcmp(argv[i], "-rerun") == 0 && i+1 < argc) {
        test_iters += strToUInt(argv[++i]);
      } else {
        print("Error: Invalid option '{}'\n", argv[i]);
        printUsage();
        return EXIT_FAILURE;
      } 
    }
  } catch (std::exception &e) {
    print("\n{}\n", e.what());    
    printUsage();
    return EXIT_FAILURE;
  }

  char *file_buff = nullptr;
  size_t file_buff_size = 0;
  if (use_setvbuf) {
    setvbuf(stdout, console_buff, _IOFBF, console_buff_size);
    file_buff = ::file_buff;
    file_buff_size = ::file_buff_size;
  }

  Test console_tests[] = {
    Test(printfTest, console_test_iters, "printf"),
    Test(coutTest, console_test_iters, "cout"),
    Test(vtemplate_printfTest, console_test_iters, 
         "C++11 variadic print with printf"),
    Test(vtemplate_coutTest, console_test_iters, 
         "C++11 variadic print with cout"),
    Test(KameUtil_coutTest, console_test_iters, "KameUtil::print with cout")
  };

  Test string_tests[] = {
    // string tests
    Test(snprintfTest, string_test_iters, "snprintf"),
    Test(stringstreamTest, string_test_iters, "stringstream"),
    Test(vtemplate_snprintfTest, string_test_iters, 
         "C++11 variadic print with snprintf"),
    Test(vtemplate_sstreamTest, string_test_iters, 
         "C++11 variadic print with stringstream"),
    Test(KameUtil_snprintfTest, string_test_iters,
         "KameUtil::streamPrint with snprintf"),
    Test(KameUtil_sstreamTest, string_test_iters, 
         "KameUtil::streamPrint with std::stringstream")
  };

  FileTest file_tests[] = {
    // formatted file tests
    FileTest(fprintfTest, file_test_iters, "fprintf", 
             file_buff, file_buff_size),
    FileTest(buffered_fwriteTest, file_test_iters, "buffered fwrite",
             file_buff, file_buff_size),
    FileTest(ofstreamTest, file_test_iters, "ofstream", file_buff, 
             file_buff_size), 
    FileTest(buffered_ofstreamWriteTest, file_test_iters, 
             "buffered ofstream write", file_buff, file_buff_size), 
    FileTest(vtemplate_fprintfTest, file_test_iters, 
             "C++11 variadic print with fprintf", file_buff, 
             file_buff_size),
    FileTest(vtemplate_ofstreamTest, file_test_iters, 
             "C++11 variadic print with ofstream", file_buff, 
             file_buff_size), 
    FileTest(KameUtil_fprintfTest, file_test_iters, 
             "KameUtil::streamPrint with fprintf", file_buff, 
             file_buff_size),
    FileTest(KameUtil_ofstreamTest, file_test_iters, 
             "KameUtil::streamPrint with ofstream", file_buff, 
             file_buff_size),
    FileTest(KameUtil_buffered_fwriteTest, file_test_iters, 
             "KameUtil::streamPrint with buffered fwrite", file_buff, 
             file_buff_size),

    // binary file tests
    FileTest(binary_fwriteTest, file_test_iters, "binary fwrite", 
             file_buff, file_buff_size),
    FileTest(bufferedBinary_fwriteTest, file_test_iters,
             "buffered binary fwrite", file_buff, file_buff_size),
    FileTest(binary_ofstreamWriteTest, file_test_iters, 
             "binary ofstream write", file_buff, file_buff_size),
    FileTest(bufferedBinary_ofstreamWriteTest, file_test_iters, 
             "buffered binary ofstream write", file_buff, file_buff_size),
    FileTest(KameUtil_bufferedBinary_fwriteTest, file_test_iters, 
             "KameUtil::streamPrint with buffered binary fwrite", 
             file_buff, file_buff_size),
  };

  for (size_t i = 0; i < test_iters; ++i) {
    if (console_test_iters > 0) {
      for (auto &test : console_tests) {
        print("\n{} test\n", test.test_name);
        fflush(stdout);
        std::chrono::milliseconds ms(1500);
        std::this_thread::sleep_for(ms);
        test.run();
        fflush(stdout);
        std::cout.flush();
      }
    }

    if (file_test_iters > 0) { 
      for (auto &test : file_tests) {
        print("\n{} test\n", test.test_name);
        fflush(stdout);
        test.run();
      }
    }
    
    if (string_test_iters > 0) {
      for (auto &test : string_tests) {
        print("\n{} test\n", test.test_name);
        fflush(stdout);
        test.run();
      }
    }
  }

  print("\nconsole test iterations = {}\n", console_test_iters);
  print("file test iterations = {}\n", file_test_iters);
  print("string test iterations = {}\n", string_test_iters);
  print("number of times each test was run = {}\n\n", test_iters);

  if (console_test_iters > 0) {
    if (use_setvbuf) {
      print("console tests with buffer (setvbuf,pubsetbuf) = {} bytes:\n",
             console_buff_size);
    } else {
      print("console tests:\n");
    }

    for (auto &test : console_tests) {
      print("{} test time = {}ms\n", test.test_name, test.average());
    }
  }

  if (file_test_iters > 0) {
    if (use_setvbuf) {
      print("\nfile tests with buffer (setvbuf,pubsetbuf) = {} bytes:\n",
             file_buff_size);
    } else {
      print("\nfile tests:\n");
    }

    for (auto &test : file_tests) {
      print("{} test time = {}ms\n", test.test_name, test.average());
    }
  }

  if (string_test_iters > 0) {
    print("\nstring tests:\n");
    for (auto &test : string_tests) 
      print("{} test time = {}ms\n", test.test_name, test.average());
  }

  return EXIT_SUCCESS;
}

void printUsage()
{
  using std::cout;
  cout << "Usage: print-tests [--help] [-no-setvbuf] [-console COUNT] "
          "[-file COUNT]\n\t[-string COUNT] [-rerun COUNT]\n"
          "-no-setvbuf = Don't use setvbuf for console and files.\n"
          "-console COUNT = Run console tests COUNT times. This affects "
          "how much data is\n\twritten to console in each test.\n"
          "\tDefault is 5000.\n"
          "-file COUNT = Run file tests COUNT times. This affects how "
          "much data is written\n\tto the test file 'test.out' in each "
          "test.\n\tDefault is 1000000.\n"
          "-string COUNT = Run string tests COUNT times. This affects how "
          "much data is\n\twritten to the string in each test.\n"
          "\tDefault is 500000.\n"
          "-rerun COUNT = Run all tests again COUNT times for taking an "
          "average of each\n\ttest's duration.\n"
          "\tDefault is 0\n";
}

size_t strToUInt(const char *str)
{
  char *str_end = nullptr; // for detecting errors in strtol
  int n = strtol(str, &str_end, 10);
  if (n < 0) {
    throw std::runtime_error("Error: Negative number entered.");
  }
  if (errno == ERANGE){
    errno = 0;
    throw std::runtime_error("Error: Number entered is too large.");
  }
  if (str == str_end) {
    throw std::runtime_error("Error converting string to int");
  }
  return n;
}
 
