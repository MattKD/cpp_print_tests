# cpp_print_tests
A bunch of stdio, iostream, and string performance tests.

Use make or the Visual Studio 2015 project to compile, which will produce the 
executable print-tests.

To use run "print-tests [--help] [-no-setvbuf] [-console COUNT] [-file COUNT] [-string COUNT] [-rerun COUNT]"

---
KameUtil-Print is a safe C++11 template based print function supporting positional arguments: https://github.com/MattKD/KameUtil-Print

The results listed as "C++11 variadic print" are using a recursive variadic template version from here: https://en.wikipedia.org/wiki/Variadic_template

---

Results on Windows 7/Intel i7-4770K compiled with Visual Studio 2015:
```
console test iterations = 5000
file test iterations = 1000000
string test iterations = 500000
number of times each test was run = 10

console tests with buffer (setvbuf,pubsetbuf) = 65536 bytes:
printf test time = 209.6ms
cout test time = 172.8ms
C++11 variadic print with printf test time = 207.8ms
C++11 variadic print with cout test time = 180.1ms
KameUtil::print with cout test time = 171.2ms

file tests with buffer (setvbuf,pubsetbuf) = 65536 bytes:
fprintf test time = 3296.9ms
buffered fwrite test time = 2608ms
ofstream test time = 7037.2ms
buffered ofstream write test time = 2585.1ms
C++11 variadic print with fprintf test time = 4934.4ms
C++11 variadic print with ofstream test time = 10435.6ms
KameUtil::streamPrint with fprintf test time = 5245.7ms
KameUtil::streamPrint with ofstream test time = 11301.4ms
KameUtil::streamPrint with buffered fwrite test time = 2834.8ms
binary fwrite test time = 1762.6ms
buffered binary fwrite test time = 189.9ms
binary ofstream write test time = 2588.8ms
buffered binary ofstream write test time = 189.9ms
KameUtil::streamPrint with buffered binary fwrite test time = 262.8ms

string tests:
snprintf test time = 1021ms
stringstream test time = 3318.2ms
C++11 variadic print with snprintf test time = 1289.4ms
C++11 variadic print with stringstream test time = 4524.6ms
KameUtil::streamPrint with snprintf test time = 1370.1ms
KameUtil::streamPrint with std::stringstream test time = 4774.8ms
```

Results on Windows 7/Intel i7-4770K compiled with MinGW:
```
console test iterations = 5000
file test iterations = 1000000
string test iterations = 500000
number of times each test was run = 10

console tests with buffer (setvbuf,pubsetbuf) = 65536 bytes:
printf test time = 213ms
cout test time = 170ms
C++11 variadic print with printf test time = 209ms
C++11 variadic print with cout test time = 174ms
KameUtil::print with cout test time = 171ms

file tests with buffer (setvbuf,pubsetbuf) = 65536 bytes:
fprintf test time = 6003ms
buffered fwrite test time = 2062ms
ofstream test time = 4449ms
buffered ofstream write test time = 2116ms
C++11 variadic print with fprintf test time = 6464ms
C++11 variadic print with ofstream test time = 5879ms
KameUtil::streamPrint with fprintf test time = 6728ms
KameUtil::streamPrint with ofstream test time = 6307ms
KameUtil::streamPrint with buffered fwrite test time = 2222ms
binary fwrite test time = 1691ms
buffered binary fwrite test time = 160ms
binary ofstream write test time = 2683ms
buffered binary ofstream write test time = 159ms
KameUtil::streamPrint with buffered binary fwrite test time = 291ms

string tests:
snprintf test time = 605ms
stringstream test time = 1269ms
C++11 variadic print with snprintf test time = 788ms
C++11 variadic print with stringstream test time = 1898ms
KameUtil::streamPrint with snprintf test time = 850ms
KameUtil::streamPrint with std::stringstream test time = 2049ms
```
