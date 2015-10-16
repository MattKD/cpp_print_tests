#include <cstdio>
#include <cstring>
#include <iostream>
#include <thread>
#include <chrono>
#include "print_tests.h"
#include "console_tests.h"
#include "file_tests.h"
#include "string_tests.h"

//
// buffers for stdout and files using setvbuf and pubsetbuf functions
//
const size_t stdout_buff_size = 1 << 16; //64Kb
const size_t file_buff_size = stdout_buff_size;
char stdout_buff[stdout_buff_size];
char file_buff[file_buff_size];

int main(int argc, char *argv[])
{  
  std::ios::sync_with_stdio(false);
  bool use_setvbuf_stdout = false;
  size_t test_iters = 1;
  size_t stdout_test_iters = 1000;
  size_t file_test_iters = 1000000;
  size_t string_test_iters = 500000;

  try {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "--help") == 0) {
        printUsage();
        return EXIT_SUCCESS;
      } else if (strcmp(argv[i], "-use-setvbuf") == 0) {
        use_setvbuf_stdout = true;
      } else if (strcmp(argv[i], "-console") == 0 && i+1 < argc) {
        stdout_test_iters = strToUInt(argv[++i]);
      } else if (strcmp(argv[i], "-string") == 0 && i+1 < argc) {
        string_test_iters = strToUInt(argv[++i]);
      } else if (strcmp(argv[i], "-file") == 0 && i+1 < argc) {
        file_test_iters = strToUInt(argv[++i]);
      } else if (strcmp(argv[i], "-rerun") == 0 && i+1 < argc) {
        test_iters += strToUInt(argv[++i]);
      } else {
        printf("Error: Invalid option '%s'\n", argv[i]);
        printUsage();
        return EXIT_FAILURE;
      } 
    }
  } catch (std::exception &e) {
    printf("\n%s\n", e.what());    
    printUsage();
    return EXIT_FAILURE;
  }

  if (use_setvbuf_stdout) {
    setvbuf(stdout, stdout_buff, _IOFBF, stdout_buff_size);
  }

  Test stdout_tests[] = {
    // stdout tests
    Test(printfTest, stdout_test_iters, "printf"),
    Test(coutTest, stdout_test_iters, "cout"),
    Test(templatePrintfTest, stdout_test_iters, "template printf"),
    Test(templateCppPrintfTest, stdout_test_iters, "template cpp printf"),
  };

  Test string_tests[] = {
    // string tests
    Test(sprintfTest, string_test_iters, "sprintf"),
    Test(templateSprintfTest, string_test_iters, "template sprintf"),
    Test(heapSprintfTest, string_test_iters, "heap sprintf"),
    Test(ostringstreamTest, string_test_iters, "ostringstream"),
    Test(templateHeapSprintfTest, string_test_iters, "template heap sprintf")
  };


  FileTest file_tests[] = {
    // formatted file tests
    FileTest(fprintfTest, file_test_iters, "fprintf"),
    FileTest(buffered_fwriteTest, file_test_iters, 
             "buffered (formatted) fwrite"),
    FileTest(ofstreamTest, file_test_iters, "ofstream"), 
    FileTest(buffered_ofstreamWriteTest, file_test_iters, 
             "buffered (formatted) ofstream write"), 
    FileTest(templateFPrintfTest, file_test_iters, "template fprintf"),
    FileTest(templateCppFPrintfTest, file_test_iters, "template cpp fprintf"), 
    // binary file tests
    FileTest(binary_fwriteTest, file_test_iters, "binary fwrite"),
    FileTest(bufferedBinary_fwriteTest, file_test_iters,
             "buffered binary fwrite"),
    FileTest(binary_ofstreamWriteTest, file_test_iters, 
             "binary ofstream write"),
    FileTest(bufferedBinary_ofstreamWriteTest, file_test_iters, 
             "buffered binary ofstream write")
  };

  // File tests using setvbuf/ofstream::pubsetbuf
  FileTest file_tests2[] = {
    // formatted file tests
    FileTest({fprintfTest, file_buff, file_buff_size}, file_test_iters,
             "fprintf"),
    FileTest({buffered_fwriteTest,  file_buff, file_buff_size},
             file_test_iters, "buffered (formatted) fwrite"),
    FileTest({ofstreamTest,  file_buff, file_buff_size}, 
             file_test_iters, "ofstream"), 
    FileTest({buffered_ofstreamWriteTest,  file_buff, file_buff_size},
             file_test_iters, "buffered (formatted) ofstream write"), 
    FileTest({templateFPrintfTest,  file_buff, file_buff_size}, 
             file_test_iters,"template fprintf"),
    FileTest({templateCppFPrintfTest, file_buff, file_buff_size},  
             file_test_iters,"template cpp fprintf"), 
    // binary file tests
    FileTest({binary_fwriteTest,  file_buff, file_buff_size}, 
             file_test_iters,"binary fwrite"),
    FileTest({bufferedBinary_fwriteTest, file_buff, file_buff_size},  
             file_test_iters,"buffered binary fwrite"),
    FileTest({binary_ofstreamWriteTest,  file_buff, file_buff_size}, 
             file_test_iters,"binary ofstream write"),
    FileTest({bufferedBinary_ofstreamWriteTest,  file_buff, file_buff_size},
             file_test_iters, "buffered binary ofstream write")
  };

  for (size_t i = 0; i < test_iters; ++i) {
    if (stdout_test_iters > 0) {
      for (auto &test : stdout_tests) {
        printf("\n%s test\n", test.test_name);
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
        printf("\n%s test\n", test.test_name);
        fflush(stdout);
        test.run();
      }
      
      for (auto &test : file_tests2) {
        printf("\n%s test (with setvbuf/ofstream::pubsetbuf\n", 
               test.test_name);
        fflush(stdout);
        test.run();
      }
    }
    
    if (string_test_iters > 0) {
      for (auto &test : string_tests) {
        printf("\n%s test\n", test.test_name);
        fflush(stdout);
        test.run();
      }
    }


  }

  printf("\nstdout test iterations = %zu\n", stdout_test_iters);
  printf("file test iterations = %zu\n", file_test_iters);
  printf("string test iterations = %zu\n", string_test_iters);
  printf("number of times each test was run = %zu\n\n", test_iters);

  if (use_setvbuf_stdout)
    printf("stdout tests with buffer (setvbuf,pubsetbuf) = %zu bytes:\n",
           stdout_buff_size);
  else 
    printf("stdout tests:\n");

  for (auto &test : stdout_tests) 
    printf("%s test time = %.2fms\n", test.test_name, test.average());

  printf("\nfile tests:\n");
  for (auto &test : file_tests) 
    printf("%s test time = %.2fms\n", test.test_name, test.average());

  printf("\nfile tests with buffer (setvbuf) = %zu bytes:\n",
         file_buff_size);
  for (auto &test : file_tests2) 
    printf("%s test time = %.2fms\n", test.test_name, test.average());

  printf("\nstring tests:\n");
  for (auto &test : string_tests) 
    printf("%s test time = %.2fms\n", test.test_name, test.average());

  return EXIT_SUCCESS;
}

void printUsage()
{
  printf("Usage: print-tests [--help] [-use-setvbuf] [-console COUNT] "
         "[-file COUNT]\n\t[-string COUNT] [-rerun COUNT]\n");
  printf("-use-setvbuf = Use setvbuf for stdout.\n");
  printf("-console COUNT = Run console tests COUNT times. This affects "
         "how much data is\n\twritten to console in each test.\n"
         "\tDefault is 1000.\n");
  printf("-file COUNT = Run file tests COUNT times. This affects how "
         "much data is written\n\tto the test file 'test.out' in each "
         "test.\n\tDefault is 1000000.\n");
  printf("-string COUNT = Run string tests COUNT times. This affects how "
         "much data is\n\twritten to the string in each test.\n"
         "\tDefault is 500000.\n");
  printf("-rerun COUNT = Run all tests again COUNT times for taking an "
         "average of each\n\ttest's duration.\n"
         "\tDefault is 0.\n");
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
 
