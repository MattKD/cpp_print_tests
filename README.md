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
number of times each test was run = 11

stdout tests with buffer (setvbuf,pubsetbuf) = 65536 bytes:
printf test time = 168.00ms
cout test time = 145.27ms
template printf test time = 129.45ms
template cpp printf test time = 161.00ms

file tests:
fprintf test time = 7523.91ms
buffered (formatted) fwrite test time = 5741.64ms
ofstream test time = 16942.91ms
buffered (formatted) ofstream write test time = 5786.18ms
template fprintf test time = 10508.91ms
template cpp fprintf test time = 22311.00ms
binary fwrite test time = 4052.73ms
buffered binary fwrite test time = 1321.73ms
binary ofstream write test time = 5441.45ms
buffered binary ofstream write test time = 1390.91ms

file tests with buffer (setvbuf) = 65536 bytes:
fprintf test time = 7424.09ms
buffered (formatted) fwrite test time = 5625.09ms
ofstream test time = 16598.55ms
buffered (formatted) ofstream write test time = 5644.64ms
template fprintf test time = 10329.18ms
template cpp fprintf test time = 21700.36ms
binary fwrite test time = 3922.09ms
buffered binary fwrite test time = 1299.09ms
binary ofstream write test time = 5325.91ms
buffered binary ofstream write test time = 1396.55ms

string tests:
sprintf test time = 2743.64ms
template sprintf test time = 2431.64ms
heap sprintf test time = 2663.36ms
ostringstream test time = 7505.82ms
template heap sprintf test time = 2754.18ms
```
Results on Windows 7/Intel i7-4770K compiled with Visual Studio 2015:
```
stdout test iterations = 1000
file test iterations = 1000000
string test iterations = 500000
number of times each test was run = 11

stdout tests with buffer (setvbuf,pubsetbuf) = 65536 bytes:
printf test time = 247.27ms
cout test time = 87.55ms
template printf test time = 81.09ms
template cpp printf test time = 91.45ms

file tests:
fprintf test time = 3348.64ms
buffered (formatted) fwrite test time = 2863.55ms
ofstream test time = 8008.45ms
buffered (formatted) ofstream write test time = 2910.45ms
template fprintf test time = 4985.82ms
template cpp fprintf test time = 10583.82ms
binary fwrite test time = 1784.64ms
buffered binary fwrite test time = 483.45ms
binary ofstream write test time = 2443.45ms
buffered binary ofstream write test time = 512.00ms

file tests with buffer (setvbuf) = 65536 bytes:
fprintf test time = 3339.18ms
buffered (formatted) fwrite test time = 2878.09ms
ofstream test time = 7976.55ms
buffered (formatted) ofstream write test time = 2872.73ms
template fprintf test time = 4972.45ms
template cpp fprintf test time = 10534.91ms
binary fwrite test time = 1765.45ms
buffered binary fwrite test time = 475.27ms
binary ofstream write test time = 2292.36ms
buffered binary ofstream write test time = 467.36ms

string tests:
sprintf test time = 1502.55ms
template sprintf test time = 1348.91ms
heap sprintf test time = 1436.91ms
ostringstream test time = 3720.55ms
template heap sprintf test time = 1450.36ms
```
