#ifndef KAME_UTIL_STREAM_PRINT_H
#define KAME_UTIL_STREAM_PRINT_H

#include <cctype>
#include <initializer_list>

namespace KameUtil {

namespace impl {

template <class OStream, class T>
struct PrintGenerator {
  static void print(void *ostream, const void *data)
  {
    OStream &os = *(OStream*)ostream;
    os << *(T*)data;
  }
};

struct ErasedPrintable {
  const void *data;
  void (*print)(void *ostream, const void *data);
};

template <class OStream, class T>
ErasedPrintable makeErased(const T &val)
{
  ErasedPrintable ep = { (void*)&val, PrintGenerator<OStream, T>::print };
  return ep;
}

// Parse integer in range 0 to 9999
// Precondition: fmt[0] must be a digit, no negatives
template <class CharT>
bool parseInt(const CharT *&fmt, int &out_int)
{
  const int max_num_digits = 4; // 9999 max
  int digits[max_num_digits];

  int c = *fmt;

  // if first digit is 0 then return 0; leading 0's not allowed
  if (c == '0') {
    ++fmt;
    out_int = 0;
    return true;
  }

  // wasn't 0, so next digit must be 1-9 if precondition was held
  int num_digits = 0;
  do {
    if (num_digits >= max_num_digits) {
      return false;
    }
    digits[num_digits++] = c - 48;
    c = *++fmt; 
  } while (isdigit(c));

  int digit_idx = num_digits-1;
  int tmp_idx = digits[digit_idx--];
  int mult = 10;
  for (; digit_idx >= 0; --digit_idx) {
    tmp_idx += digits[digit_idx] * mult;
    mult *= 10;
  }

  out_int = tmp_idx;
  return true;
}

// Parse idx in range 0-9999 from string "INT}"; no leading 0's
// Precondition: fmt[0] must be a digit; fmt[-1] is '{'
// Postcondition: On success fmt[-1] = '}'
template <class CharT>
bool parseFmtIdx(const CharT *&fmt, int &out_idx)
{
  int c = *fmt;
  // cannot be negative
  if (isdigit(c)) {
    if (!parseInt(fmt, out_idx)) {
      return false;
    }
  } else {
    return false;
  }

  c = *fmt;
  if (c != '}') {
    return false;
  }

  ++fmt; // move past '}'
  return true;
}

template <class OStream, class CharT>
bool print(OStream &os, const CharT *fmt, 
           std::initializer_list<ErasedPrintable> args)
{
  const ErasedPrintable *args_arr = args.begin();
  int idx = 0;

  CharT c = *fmt++;
  while (c != '\0') {
    if (c == '{') {
      if (*fmt == '{') { // escape format options
        os << '{';
        c = fmt[1];
        fmt += 2;
        continue;
      }

      if (*fmt == '}') {
        ++fmt;
      } else {
        if (!parseFmtIdx(fmt, idx)) {
          return false;
        }
      }
      if (idx < (int)args.size()) {
        ErasedPrintable ep = args_arr[idx++];
        ep.print(&os, ep.data);
      } else {
        return false;
      }
    } else {
      os << c;
    }
    c = *fmt++;
  }
  return true;
}

} // end namespace impl

template <class OStream, class CharT, class ...Args>
bool streamPrint(OStream &os, const CharT *fmt, const Args &...args)
{
  return impl::print(os, fmt, {impl::makeErased<OStream>(args)...});
}

} // end namespace KameUtil
#endif
