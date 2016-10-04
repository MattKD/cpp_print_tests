#include "print_tests.h"
#include "vtemplate_print.h"
#include "kame_util_fwd.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <memory>

namespace {

// Safe wrapper around cstdio FILE*
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

  ~FileWrapper() { if (file) fclose(file); file = nullptr; }

  FILE* get() { return file; }
};

// For using fprintf with template function tests
struct FprintfFwd {
  explicit FprintfFwd(const char *filename) : file{filename} { }

  template <typename... Args>
  void operator()(Args&& ...args)
  {
    fprintf(file.get(), std::forward<Args>(args)...);
  }

  void flush() { fflush(file.get()); }

  FileWrapper file;
};

// Stream object to test cstdio file functions with variadic template print
// and KameUtil-Print
struct CSTDIO_Stream {
  explicit CSTDIO_Stream(const char *filename) : file{filename} { }

  CSTDIO_Stream& operator<<(char c) 
  { 
    fputc(c, file.get()); 
    return *this; 
  }

  CSTDIO_Stream& operator<<(int i) 
  { 
    fprintf(file.get(), "%d", i); 
    return *this; 
  }

  CSTDIO_Stream& operator<<(double d) 
  { 
    fprintf(file.get(), "%f", d); 
    return *this; 
  }

  CSTDIO_Stream& operator<<(const char *s) 
  { 
    fputs(s, file.get()); 
    return *this; 
  }

  void flush() { fflush(file.get()); }

  FileWrapper file;
};

// Wrapper for fwrite using same function signature as ofstream::write
// to be used with BinaryOutStream and BufferedBinaryOutStream
struct FwriteFwd {
  explicit FwriteFwd(const char *filename) : file{filename} { }

  void write(const char *data, size_t count) 
  { 
    fwrite(data, 1, count, file.get()); 
  }

  void flush() { fflush(file.get()); }

  FileWrapper file;
};

// Stream object for testing fwrite and ostream::write with formatted text
template <typename OutStream>
struct BufferedOutStream {
  BufferedOutStream(OutStream &&os) : os{std::move(os)}, 
    buffer{new char[buffer_max_size]}, buffer_size{0} {}

  BufferedOutStream(BufferedOutStream &&other) : os{std::move(other.os)}, 
    buffer{std::move(other.buffer)}, buffer_size{other.buffer_size} 
  {
    other.buffer_size = 0;
  }

  ~BufferedOutStream() 
  { 
    if (buffer_size > 0) {
      write();
    }
  }

  void write()
  {
    os.write(buffer.get(), buffer_size);
    buffer_size = 0;
  }

  void flush() { write(); os.flush(); }

  void checkSize(int needed) 
  {
    if (buffer_size + needed > buffer_max_size) {
      write();
    }
  }

  BufferedOutStream& operator<<(const char *str)
  {
    char c = *str;
    while (c != '\0') {
      if (buffer_size == buffer_max_size) {
        write();
      }
      buffer[buffer_size++] = c;
      c = *++str;
    }
    return *this;
  }

  BufferedOutStream& operator<<(char c)
  {
    if (buffer_size == buffer_max_size) {
      write();
    }
    buffer[buffer_size++] = c;
    return *this;
  }

  BufferedOutStream& operator<<(int i)
  {
    const int max_int_size = 64;
    char int_str[max_int_size];
    int n = snprintf(int_str, max_int_size, "%d", i);
    checkSize(n);
    memcpy(buffer.get()+buffer_size, int_str, n);
    buffer_size += n;
    return *this;
  }

  BufferedOutStream& operator<<(double d)
  {
    const int dbl_str_size = 2048;
    char dbl_str[dbl_str_size];
    int n = snprintf(dbl_str, dbl_str_size, "%f", d);
    checkSize(n);
    memcpy(buffer.get()+buffer_size, dbl_str, n);
    buffer_size += n;
    return *this;
  }

  OutStream os;
  const size_t buffer_max_size = 1 << 14; //16Kb
  std::unique_ptr<char[]> buffer;
  size_t buffer_size;
};

// Stream object for testing fwrite and ostream::write with binary data
template <typename OutStream>
struct BinaryOutStream {
  BinaryOutStream(OutStream os) : os{std::move(os)} {}

  BinaryOutStream& operator<<(const char *str)
  {
    os.write(str, strlen(str));
    return *this;
  }

  template <class T>
  BinaryOutStream& operator<<(T val)
  {
    os.write((char*)&val, sizeof(T)); 
    return *this;
  }

  void flush() { os.flush(); }

  OutStream os;
};

// Template for writing binary data to a stream using a buffer.
// Data is written only when buffer is filled.
template <typename OutStream>
struct BufferedBinaryOutStream {
  BufferedBinaryOutStream(OutStream &&os) : os{std::move(os)}, 
    buffer{new char[buffer_max_size]}, buffer_size{0} {}

  BufferedBinaryOutStream(BufferedBinaryOutStream &&other) 
    : os{std::move(other.os)}, buffer{std::move(other.buffer)}, 
      buffer_size{other.buffer_size} 
  {
    other.buffer_size = 0;
  }

  ~BufferedBinaryOutStream() 
  { 
    if (buffer_size > 0) {
      write();
    }
  }

  void write()
  {
    os.write(buffer.get(), buffer_size);
    buffer_size = 0;
  }

  void flush() { write(); os.flush(); }

  BufferedBinaryOutStream& operator<<(const char *str)
  {
    char c = *str;
    while (c != '\0') {
      if (buffer_size == buffer_max_size) {
        write();
      }
      buffer[buffer_size++] = c;
      c = *++str;
    }
    return *this;
  }

  BufferedBinaryOutStream& operator<<(char c)
  {
    if (buffer_size == buffer_max_size) {
      write();
    }
    buffer[buffer_size++] = c;
    return *this;
  }

  BufferedBinaryOutStream& operator<<(int i)
  {
    checkSize(sizeof(i));
    memcpy(buffer.get()+buffer_size, &i, sizeof(i));
    buffer_size += sizeof(i);
    return *this;
  }

  BufferedBinaryOutStream& operator<<(double d)
  {
    checkSize(sizeof(d));
    memcpy(buffer.get()+buffer_size, &d, sizeof(d));
    buffer_size += sizeof(d);
    return *this;
  }

  void checkSize(int needed)
  {
    if (buffer_size + needed > buffer_max_size) {
      write();
    }
    buffer_size = 0;
  }

  OutStream os;
  const size_t buffer_max_size = 1 << 14;
  std::unique_ptr<char[]> buffer;
  size_t buffer_size;
};

} // end anon namespace

// Tests fprintf
double fprintfTest(size_t iterations, char *buff, size_t buff_size)
{
  FprintfFwd fout("test.out");
  if (buff) {
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);
  }
  return printfStyleTest(fout, iterations);
}

// Tests fwrite with formatted text and a large buffer
double buffered_fwriteTest(size_t iterations, char *buff, size_t buff_size)
{
  FwriteFwd fout("test.out");
  if (buff)
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);

  BufferedOutStream<FwriteFwd> os(std::move(fout));

  return cppStyleTest(os, iterations);
}

// Tests fwrite with binary data
double binary_fwriteTest(size_t iterations, char *buff, size_t buff_size)
{
  FwriteFwd fout("test.out");
  if (buff)
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);

  BinaryOutStream<FwriteFwd> os{std::move(fout)};
  return cppStyleTest(os, iterations);
}

// Tests fwrite with binary data and a large buffer
double bufferedBinary_fwriteTest(size_t iterations, char *buff, size_t buff_size)
{
  FwriteFwd fout("test.out");
  if (buff)
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);

  BufferedBinaryOutStream<FwriteFwd> os(std::move(fout));
  return cppStyleTest(os, iterations);
}

// Tests std::ofstream
double ofstreamTest(size_t iterations, char *buff, size_t buff_size)
{
  std::ofstream fout("test.out");
  if (buff)
    fout.rdbuf()->pubsetbuf(buff, buff_size);
  return cppStyleTest(fout, iterations);
}

// Tests std::ofstream::write with formatted text and a large buffer
double buffered_ofstreamWriteTest(size_t iterations, char *buff, 
  size_t buff_size)
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

// Tests templated Fprintf using stdio FILE functions
double vtemplate_fprintfTest(size_t iterations, char *buff, 
                             size_t buff_size)
{
  CSTDIO_Stream fout("test.out");
  if (buff)
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);

  VTemplatePrintFwd<CSTDIO_Stream> func{std::move(fout)};

  return printfStyleTest(func, iterations);
}

// Tests variadic template print with ofstream
double vtemplate_ofstreamTest(size_t iterations, char *buff, 
                              size_t buff_size)
{
  std::ofstream fout("test.out");
  if (buff)
    fout.rdbuf()->pubsetbuf(buff, buff_size);
  VTemplatePrintFwd<std::ofstream> func{std::move(fout)};
  return printfStyleTest(func, iterations);
}

// Tests KameUtil::streamPrint with ofstream
double KameUtil_ofstreamTest(size_t iterations, char *buff, 
                             size_t buff_size)
{
  std::ofstream fout("test.out");
  if (buff) {
    fout.rdbuf()->pubsetbuf(buff, buff_size);
  }
  KameUtilPrintFwd<std::ofstream> func(std::move(fout));
  return KameUtilPrintStyleTest(func, iterations);
}

// Tests KameUtil::streamPrint with fprintf
double KameUtil_fprintfTest(size_t iterations, char *buff, size_t buff_size)
{
  CSTDIO_Stream fout("test.out");
  if (buff) {
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);
  }

  KameUtilPrintFwd<CSTDIO_Stream> func{std::move(fout)};
  return KameUtilPrintStyleTest(func, iterations);
}

// Tests fwrite with formatted text and a large buffer
double KameUtil_buffered_fwriteTest(size_t iterations, char *buff, 
                                   size_t buff_size)
{
  FwriteFwd fout("test.out");
  if (buff)
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);

  typedef BufferedOutStream<FwriteFwd> OS;
  auto func = KameUtilPrintFwd<OS>(OS(std::move(fout)));
  return KameUtilPrintStyleTest(func, iterations);
}

// Tests fwrite with binary data and a large buffer
double 
KameUtil_bufferedBinary_fwriteTest(size_t iterations, char *buff, 
                                  size_t buff_size)
{
  FwriteFwd fout("test.out");
  if (buff)
    setvbuf(fout.file.get(), buff, _IOFBF, buff_size);

  typedef BufferedBinaryOutStream<FwriteFwd> OS;
  auto func = KameUtilPrintFwd<OS>(OS(std::move(fout)));
  return KameUtilPrintStyleTest(func, iterations);
}



