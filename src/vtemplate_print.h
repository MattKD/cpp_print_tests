#ifndef TEMPLATE_FPRINTF_H
#define TEMPLATE_FPRINTF_H

#include <utility>

// Test the templated versions of printf using a simple version
// adapted from wikipedia: https://en.wikipedia.org/wiki/Variadic_template. 
// The templated version wouldn't need the 
// conversion chars in the format string, but keeping them allow for
// reusing the format strings from other tests.
template<typename OS>
void vtemplatePrint(OS &os, const char *s)
{
  const char *s2 = s;
  while (*s2) {
    if (*s2 == '%') {
      if (*(s2 + 1) == '%') {
          ++s2;
      } else {
        //assert
      }
    }
    ++s2;
  }
  os << s;
}
 
template<typename OS, typename T, typename... Args>
void vtemplatePrint(OS &os, const char *s, T &&value, Args&&... args)
{
  while (*s) {
    if (*s == '%') {
      if (*(s + 1) == '%') {
        ++s;
      }
      else {
        os << value;
        s += 2; // ignore conversion char
        vtemplatePrint(os, s, std::forward<Args>(args)...);
        return;
      }
    }
    os << *s++;
  }    
}

// To pass vtemplatePrint template function to template test functions
template <typename OutStream>
struct VTemplatePrintFwd {
  OutStream os;
  VTemplatePrintFwd(OutStream os) : os{std::move(os)} {}

  template <typename... Args>
  void operator()(Args&& ...args)
  {
    vtemplatePrint(os, std::forward<Args>(args)...);
  }

  void flush() { os.flush(); }
};

#endif
