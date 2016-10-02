#ifndef STRING_H
#define STRING_H

#include <cstdarg>
#include <memory>
#include <cstring>

class String {
  size_t sz;
  size_t buff_size;
  std::unique_ptr<char[]> buff;

public:
  String() : sz{0}, buff_size{20}, buff{new char[buff_size]} 
  { 
    buff[0] = '\0'; 
  }
  size_t size() { return sz; }
  const char* c_str() { return buff.get(); }

  void append(const char *src, size_t src_size)
  {
    if ((sz + src_size) >= (buff_size - 1)) {
      do {
        buff_size *= 2;       
      } while ((sz + src_size) >= (buff_size - 1));

      char *tmp = new char[buff_size];
      strcpy(tmp, buff.get()); // buff is null terminated
      buff.reset(tmp);
    }

    strncpy(buff.get() + sz, src, src_size);
    sz += src_size;
    buff[sz] = '\0';
  }

  void append(int n)
  {
    char int_str[100];
    size_t int_str_len = ::sprintf(int_str, "%d", n);
    append(int_str, int_str_len);
  }

  void append(double d)
  {
    char dbl_str[500];
    size_t dbl_str_len = ::sprintf(dbl_str, "%f", d);
    append(dbl_str, dbl_str_len);
  }

  void append(char c)
  {
    buff[sz++] = c;

    if (sz == (buff_size - 1)) {
      buff[sz] = '\0';
      buff_size *= 2;
      char *tmp = new char[buff_size];
      strcpy(tmp, buff.get());
      buff.reset(tmp);
    }
    buff[sz] = '\0';
  }

  void append(const char *str)
  {
    size_t str_len = strlen(str);
    append(str, str_len);
  }

  void sprintf(const char *fmt, ...)
  {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
      if (*fmt == '%') {
        switch (*++fmt) {
          case '%': {
            append(*fmt);
            break;
          }
          case 'd': {
            int n = va_arg(args, int);
            append(n);
            break;
          }
          case 'f': {
            double d = va_arg(args, double);
            append(d);
            break;
          }
          case 's': {
            const char *str = va_arg(args, const char *);
            append(str);
            break;
          }
        }
      } else {
        append(*fmt);
      }

      fmt++;
    }

    va_end(args);
  }
};

#endif
