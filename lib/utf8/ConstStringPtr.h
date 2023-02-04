#pragma once

#include <string>

namespace utf8
{
  template<typename TStr, typename TChar>
  class ConstStringPtr
  {
    TStr Str;
    const TChar* Ptr;

  public:
    ConstStringPtr(const TChar* ptr)
      : Str()
      , Ptr(ptr)
    {
    }

    ConstStringPtr(const TChar* ptr, size_t n)
      : Str(ptr, n)
      , Ptr(Str.c_str())
    {
    }

    ConstStringPtr(const TStr& str)
      : Str(str)
      , Ptr(Str.c_str())
    {
    }

    operator const TChar* () const
    {
      return Ptr;
    }
  };

  class Utf8Ptr : public ConstStringPtr<std::string, char>
  {
    using ConstStringPtr::ConstStringPtr;
  };

  class AnsiPtr : public ConstStringPtr<std::string, char>
  {
    using ConstStringPtr::ConstStringPtr;
  };
}

#ifdef _WIN32
#define NativeStringPtr utf8::AnsiPtr
#else
#define NativeStringPtr utf8::Utf8Ptr
#endif