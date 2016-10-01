# cpp_print_tests
A bunch of stdio, iostream, and string performance tests.

Use make or the Visual Studio 2015 project to compile, which will produce the 
executable print-tests.

To use run "print-tests [--help] [-use-setvbuf] [-console COUNT] [-file COUNT] [-string COUNT] [-rerun COUNT]"

Results on Windows 7/Intel i3-2370M compiled with Visual Studio 2015:
```
stdout test iterations = 1000
file test iterations = 1000000
string test iterations = 500000
number of times each test was run = 5

stdout tests with buffer (setvbuf,pubsetbuf) = 65536 bytes:
printf test time = 18.40ms
cout test time = 32.60ms
template printf test time = 19.40ms
template cpp printf test time = 48.40ms
KameUtil::print test time = 41.00ms

file tests:
fprintf test time = 6827.80ms
buffered (formatted) fwrite test time = 4531.00ms
ofstream test time = 15312.40ms
buffered (formatted) ofstream write test time = 4781.20ms
template fprintf test time = 9636.80ms
template cpp fprintf test time = 20484.00ms
KameUtil::fprint test time = 22530.00ms
binary fwrite test time = 3966.00ms
buffered binary fwrite test time = 1362.00ms
binary ofstream write test time = 5221.20ms
buffered binary ofstream write test time = 1328.80ms

file tests with buffer (setvbuf) = 65536 bytes:
fprintf test time = 6751.40ms
buffered (formatted) fwrite test time = 4485.00ms
ofstream test time = 15129.20ms
buffered (formatted) ofstream write test time = 4545.20ms
template fprintf test time = 9614.20ms
template cpp fprintf test time = 20077.00ms
KameUtil::fprint test time = 22188.00ms
binary fwrite test time = 3931.00ms
buffered binary fwrite test time = 1682.00ms
binary ofstream write test time = 5111.00ms
buffered binary ofstream write test time = 1247.20ms

string tests:
sprintf test time = 1942.40ms
template sprintf test time = 1919.20ms
heap sprintf test time = 2176.80ms
ostringstream test time = 6993.80ms
template heap sprintf test time = 2154.80ms
KameUtil::sprint test time = 9591.80ms

```
Results on Windows 7/Intel i7-4770K compiled with Visual Studio 2015:
```
stdout test iterations = 1000
file test iterations = 1000000
string test iterations = 500000
number of times each test was run = 5

stdout tests with buffer (setvbuf,pubsetbuf) = 65536 bytes:
printf test time = 4.00ms
cout test time = 9.00ms
template printf test time = 5.00ms
template cpp printf test time = 10.60ms
KameUtil::print test time = 11.20ms

file tests:
fprintf test time = 3248.20ms
buffered (formatted) fwrite test time = 2424.40ms
ofstream test time = 8105.20ms
buffered (formatted) ofstream write test time = 2430.80ms
template fprintf test time = 5081.60ms
template cpp fprintf test time = 10698.40ms
KameUtil::fprint test time = 11527.00ms
binary fwrite test time = 1822.00ms
buffered binary fwrite test time = 472.00ms
binary ofstream write test time = 2451.80ms
buffered binary ofstream write test time = 481.60ms

file tests with buffer (setvbuf) = 65536 bytes:
fprintf test time = 3202.20ms
buffered (formatted) fwrite test time = 2399.80ms
ofstream test time = 8059.20ms
buffered (formatted) ofstream write test time = 2410.00ms
template fprintf test time = 5033.60ms
template cpp fprintf test time = 10666.60ms
KameUtil::fprint test time = 11499.00ms
binary fwrite test time = 1806.60ms
buffered binary fwrite test time = 457.40ms
binary ofstream write test time = 2418.60ms
buffered binary ofstream write test time = 457.80ms

string tests:
sprintf test time = 1096.40ms
template sprintf test time = 1116.60ms
heap sprintf test time = 1213.20ms
ostringstream test time = 3768.40ms
template heap sprintf test time = 1192.40ms
KameUtil::sprint test time = 4971.80ms

```
