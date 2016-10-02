# cpp_print_tests
A bunch of stdio, iostream, and string performance tests.

Use make or the Visual Studio 2015 project to compile, which will produce the 
executable print-tests.

To use run "print-tests [--help] [-use-setvbuf] [-console COUNT] [-file COUNT] [-string COUNT] [-rerun COUNT]"

Results on Windows 7/Intel i3-2370M compiled with Visual Studio 2015:
```
stdout test iterations = 5000
file test iterations = 1000000
string test iterations = 500000
number of times each test was run = 10

stdout tests with buffer (setvbuf,pubsetbuf) = 65536 bytes:
printf test time = 268.80ms
cout test time = 250.30ms
C++11 variadic printf test time = 280.70ms
C++11 variadic printf with cout test time = 257.20ms
KameUtil::print with cout test time = 250.40ms

file tests with buffer (setvbuf,pubsetbuf) = 65536 bytes:
fprintf test time = 4833.10ms
buffered (formatted) fwrite test time = 4135.00ms
ofstream test time = 12027.80ms
buffered (formatted) ofstream write test time = 4097.30ms
C++11 variadic fprintf test time = 6943.50ms
C++11 variadic fprintf with ofstream test time = 15871.60ms
KameUtil::streamPrint with ofstream test time = 16530.00ms
KameUtil::streamPrint with fprintf test time = 7526.00ms
binary fwrite test time = 2996.20ms
buffered binary fwrite test time = 1064.80ms
binary ofstream write test time = 3799.90ms
buffered binary ofstream write test time = 1173.40ms

string tests:
sprintf test time = 1524.40ms
heap sprintf test time = 2069.10ms
ostringstream test time = 5771.80ms
C++11 variadic static buffer sprintf test time = 1859.60ms
C++11 variadic heap sprintf test time = 1972.40ms
KameUtil::print with std::stringstream test time = 7512.40ms
KameUtil::streamPrint with static buffer stream test time = 1873.50ms
KameUtil::streamPrint with heap buffer stream test time = 1968.50ms

```
Results on Windows 7/Intel i7-4770K compiled with Visual Studio 2015:
```
stdout test iterations = 5000
file test iterations = 1000000
string test iterations = 500000
number of times each test was run = 10

stdout tests with buffer (setvbuf,pubsetbuf) = 65536 bytes:
printf test time = 128.40ms
cout test time = 109.80ms
C++11 variadic printf test time = 125.40ms
C++11 variadic printf with cout test time = 112.20ms
KameUtil::print with cout test time = 109.00ms

file tests with buffer (setvbuf,pubsetbuf) = 65536 bytes:
fprintf test time = 2300.30ms
buffered (formatted) fwrite test time = 2033.70ms
ofstream test time = 5647.50ms
buffered (formatted) ofstream write test time = 2061.40ms
C++11 variadic fprintf test time = 3483.60ms
C++11 variadic fprintf with ofstream test time = 7640.20ms
KameUtil::streamPrint with ofstream test time = 8283.30ms
KameUtil::streamPrint with fprintf test time = 3780.40ms
binary fwrite test time = 1306.80ms
buffered binary fwrite test time = 500.40ms
binary ofstream write test time = 1715.20ms
buffered binary ofstream write test time = 498.30ms

string tests:
sprintf test time = 758.60ms
heap sprintf test time = 1071.60ms
ostringstream test time = 2617.80ms
C++11 variadic static buffer sprintf test time = 979.50ms
C++11 variadic heap sprintf test time = 994.80ms
KameUtil::print with std::stringstream test time = 3611.20ms
KameUtil::streamPrint with static buffer stream test time = 1024.60ms
KameUtil::streamPrint with heap buffer stream test time = 1033.30ms
```
