//          Matthew Avery Coder 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef CPP11_POLYFILL
#define CPP11_POLYFILL

#ifdef _WIN32
#include <cstdio>
#include <cstdarg>

#define THREAD_LOCAL_PF static
#define CONSTEXPR_PF inline const
#define CONSTEXPR_CONST_PF inline const
#define CONSTVAL_PF const
#define __func__ __FUNCTION__
//from http://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010
#define snprintf c99_snprintf

inline int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap)
{
  int count = -1;

  if (size != 0)
    count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
  if (count == -1)
    count = _vscprintf(format, ap);

  return count;
}

inline int c99_snprintf(char* str, size_t size, const char* format, ...)
{
  int count;
  va_list ap;

  va_start(ap, format);
  count = c99_vsnprintf(str, size, format, ap);
  va_end(ap);

  return count;
}
#else
#define  THREAD_LOCAL_PF thread_local
#define CONSTEXPR_PF constexpr
#define CONSTVAL_PF constexpr
#define CONSTEXPR_CONST_PF constexpr const
#endif


#endif
