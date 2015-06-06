#include "print_tests.h"
#include "template_fprintf.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <memory>

//
// stdio tests
//

namespace {

class FileWrapper {
  FILE *file;
public:
  explicit FileWrapper(const char *filename) 
  { 
    file = fopen(filename, "w"); 
  }
  FileWrapper(const FileWrapper&) = delete;
  FileWrapper(FileWrapper &&other) 
  { 
    file = other.file; 
    other.file = nullptr; 
  }
  ~FileWrapper() { if (file) fclose(file); }

  FILE* get() { return file; }
};

// Functor is necessary to pass fprintf to 
// printfStyleTest template function
struct fprintfFunctor {
  FileWrapper file;
  explicit fprintfFunctor(const char *filename) : file{filename} { }

  template <typename... Args>
  void operator()(Args&& ...args)
  {
    fprintf(file.get(), std::forward<Args>(args)...);
  }
};

}

// Tests fprintf
double fprintfTest(size_t iterations, char *buff, size_t buff_size)
{
  fprintfFunctor fout("test.out");
  if (buff)
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);
  return printfStyleTest(fout, iterations);
}

namespace {

// Wrapper for fwrite using same function signature as ofstream::write
// to be used with BinaryOutStream and BufferedBinaryOutStream
struct fwriteWrapper {
  FileWrapper file;
  explicit fwriteWrapper(const char *filename) : file{filename} { }
  void write(const char *data, size_t count) { fwrite(data, 1, count, file.get()); }
};

// Stream object for testing fwrite and ostream::write with formatted text
template <typename OutStream>
struct BufferedOutStream {
  OutStream os;
  const size_t buffer_max_size = 1 << 14; //16Kb
  std::unique_ptr<char[]> buffer;
  size_t buffer_size;

  BufferedOutStream(OutStream os) : os{std::move(os)}, 
    buffer{new char[buffer_max_size]}, buffer_size{0} {}

  ~BufferedOutStream() 
  { 
    if (buffer_size > 0)
      writeBuffer();
  }

  void writeBuffer()
  {
    os.write(buffer.get(), buffer_size);
    buffer_size = 0;
  }

  BufferedOutStream& operator<<(const char *str)
  {
    while (*str) {
      buffer[buffer_size++] = *str;
      if (buffer_size == buffer_max_size)
        writeBuffer();
      ++str;
    }

    return *this;
  }

  BufferedOutStream& operator<<(int i)
  {
    // max str size of int = (bits in char) * (bytes in int) * ceiling(log10(2))
    // + 1 for double to int truncation, and +2 for sign and null char
    const int max_int_size = (int)(CHAR_BIT * sizeof(int) * 0.301f) + 3;
    char int_str[max_int_size];
    sprintf(int_str, "%d", i);
    operator<<(int_str);

    return *this;
  }

  BufferedOutStream& operator<<(double d)
  {
    const int dbl_str_size = 512;
    char dbl_str[dbl_str_size];
    sprintf(dbl_str, "%f", d);
    operator<<(dbl_str);

    return *this;
  }
};

}

// Tests fwrite with formatted text and a large buffer
double buffered_fwriteTest(size_t iterations, char *buff, size_t buff_size)
{
  fwriteWrapper fout("test.out");
  if (buff)
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);

  BufferedOutStream<fwriteWrapper> os(std::move(fout));

  return cppStyleTest(os, iterations);
}

namespace {

// Stream object for testing fwrite and ostream::write with binary data
template <typename OutStream>
struct BinaryOutStream {
  OutStream os;

  BinaryOutStream(OutStream os) : os{std::move(os)} {}

  BinaryOutStream& operator<<(const char *str)
  {
    os.write(str, strlen(str));
    return *this;
  }
  BinaryOutStream& operator<<(int i)
  {
    os.write((char*)&i, sizeof(i)); 
    return *this;
  }
  BinaryOutStream& operator<<(double d)
  {
    os.write((char*)&d, sizeof(d)); 
    return *this;
  }
};

}

// Tests fwrite with binary data
double binary_fwriteTest(size_t iterations, char *buff, size_t buff_size)
{
  fwriteWrapper fout("test.out");
  if (buff)
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);

  BinaryOutStream<fwriteWrapper> os{std::move(fout)};
  return cppStyleTest(os, iterations);
}

namespace {

// Template for writing binary data to a stream using a buffer.
// Data is written only when buffer is filled.
template <typename OutStream>
struct BufferedBinaryOutStream {
  OutStream os;
  const size_t buffer_max_size = 1 << 14;
  std::unique_ptr<char[]> buffer;
  size_t buffer_size;

  BufferedBinaryOutStream(OutStream os) : os{std::move(os)}, 
    buffer{new char[buffer_max_size]}, buffer_size{0} {}

  ~BufferedBinaryOutStream() 
  { 
    if (buffer_size > 0)
      writeBuffer();
  }

  void writeBuffer()
  {
    os.write(buffer.get(), buffer_size);
    buffer_size = 0;
  }

  BufferedBinaryOutStream& operator<<(const char *str)
  {
    while (*str) {
      buffer[buffer_size++] = *str;
      if (buffer_size == buffer_max_size)
        writeBuffer();
      ++str;
    }

    return *this;
  }

  BufferedBinaryOutStream& operator<<(int i)
  {
    if ((buffer_size + sizeof(i)) >= buffer_max_size)
      writeBuffer();
    memcpy(&buffer[buffer_size], &i, sizeof(i));
    buffer_size += sizeof(i);

    return *this;
  }

  BufferedBinaryOutStream& operator<<(double d)
  {
    if ((buffer_size + sizeof(d)) >= buffer_max_size)
      writeBuffer();
    memcpy(&buffer[buffer_size], &d, sizeof(d));
    buffer_size += sizeof(d);

    return *this;
  }
};

}

// Tests fwrite with binary data and a large buffer
double bufferedBinary_fwriteTest(size_t iterations, char *buff, size_t buff_size)
{
  fwriteWrapper fout("test.out");
  if (buff)
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);

  BufferedBinaryOutStream<fwriteWrapper> os(std::move(fout));
  return cppStyleTest(os, iterations);
}

//
// iostream tests
//

// Tests std::ofstream
double ofstreamTest(size_t iterations, char *buff, size_t buff_size)
{
  std::ofstream fout("test.out");
  if (buff)
    fout.rdbuf()->pubsetbuf(buff, buff_size);
  return cppStyleTest(fout, iterations);
}

// Tests std::ofstream::write with formatted text and a large buffer
double buffered_ofstreamWriteTest(size_t iterations, char *buff, size_t buff_size)
{
  std::ofstream fout("test.out");
  if (buff)
    fout.rdbuf()->pubsetbuf(buff, buff_size);
  BufferedOutStream<std::ofstream> os(std::move(fout));
  return cppStyleTest(os, iterations);
}

// Tests std::ofstream::write with binary data
double binary_ofstreamWriteTest(size_t iterations, char *buff, size_t buff_size)
{
  std::ofstream fout("test.out");
  if (buff)
    fout.rdbuf()->pubsetbuf(buff, buff_size);
  BinaryOutStream<std::ofstream> os(std::move(fout));
  return cppStyleTest(os, iterations);
}

// Tests std::ofstream::write with binary data and a large buffer
double bufferedBinary_ofstreamWriteTest(size_t iterations, char *buff, size_t buff_size)
{
  std::ofstream fout("test.out");
  if (buff)
    fout.rdbuf()->pubsetbuf(buff, buff_size);
  BufferedBinaryOutStream<std::ofstream> os(std::move(fout));
  return cppStyleTest(os, iterations);
}

//
// templated printf style tests 
//

namespace {

// Stream object to test the templated FPrintf with stdio functions 
// to print to a file
struct OutFileStream {
  FileWrapper file;
  explicit OutFileStream(const char *filename) : file{filename} { }

  OutFileStream& operator<<(char c) { fputc(c, file.get()); return *this; }
  OutFileStream& operator<<(int i) { fprintf(file.get(), "%d", i); return *this; }
  OutFileStream& operator<<(double d) { fprintf(file.get(), "%f", d); return *this; }
  OutFileStream& operator<<(const char *s) { fputs(s, file.get()); return *this; }
};

}

// Tests templated Fprintf using stdio functions
double templateFPrintfTest(size_t iterations, char *buff, size_t buff_size)
{
  OutFileStream fout("test.out");
  if (buff)
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);

  FPrintfFunctor<OutFileStream> func{std::move(fout)};

  return printfStyleTest(func, iterations);
}

// Tests templated Fprintf using ofstream
double templateCppFPrintfTest(size_t iterations, char *buff, size_t buff_size)
{
  std::ofstream fout("test.out");
  if (buff)
    fout.rdbuf()->pubsetbuf(buff, buff_size);
  FPrintfFunctor<std::ofstream> func{std::move(fout)};
  return printfStyleTest(func, iterations);
}

