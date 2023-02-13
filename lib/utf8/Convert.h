#pragma once

#include <string>
#include <vector>

#define s8_strlen strlen
#define s8_strcpy strcpy
#define s8_strncpy strncpy
#define s8_strcmp strcmp
#define s8_strncmp strncmp

#ifdef _WIN32
  using w32string = std::basic_string<char32_t, std::char_traits<char32_t>, std::allocator<char32_t>>;
  
  #define w16_type wchar_t
  #define w32_type char32_t

  #define w16string std::wstring

  #define w16_strlen wcslen
  #define w16_strcpy wcscpy
  #define w16_strncpy wcsncpy

  inline size_t w16_strlen(const char16_t* p) { return wcslen((w16_type*)p); }
  inline w16_type* w16_strcpy(char16_t* d, const char16_t* s) { return wcscpy((w16_type*)d, (const w16_type*)s); }
  inline w16_type* w16_strncpy(char16_t* d, const char16_t* s, size_t num) { return wcsncpy((w16_type*)d, (const w16_type*)s, num); }

  size_t w32_strlen(const w32_type* p);
  w32_type* w32_strcpy(w32_type* dest, const w32_type* src);
  w32_type* w32_strncpy(w32_type* dest, const w32_type* src, size_t num);

#else 
  using w16string = std::basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>;

  #define w32string std::wstring
  #define w16_type char16_t
  #define w32_type wchar_t

  size_t w16_strlen(const w16_type* p);
  w16_type* w16_strcpy(w16_type* dest, const w16_type* src);
  w16_type* w16_strncpy(w16_type* dest, const w16_type* src, size_t num);

  #define w32_strlen wcslen
  #define w32_strcpy wcscpy
  #define w32_strncpy wcsncpy

  inline size_t w32_strlen(const char32_t* p) { return wcslen((w32_type*)p); }
  inline w32_type* w16_strcpy(char32_t* d, const char32_t* s) { return wcscpy((w32_type*)d, (const w32_type*)s); }
  inline w32_type* w16_strncpy(char32_t* d, const char32_t* s, size_t num) { return wcsncpy((w32_type*)d, (const w32_type*)s, num); }
#endif

w16_type* w16_strncpyz(w16_type* dest, const w16_type* src, size_t num);
w32_type* w32_strncpyz(w32_type* dest, const w32_type* src, size_t num);

#ifdef _WIN32
    inline w16_type* w16_strncpyz(char16_t* dest, const char16_t* src, size_t num) 
    {  return w16_strncpyz((w16_type*)dest, (const w16_type*)src, num); }
#else
    inline w32_type* w32_strncpyz(char32_t* dest, const char32_t* src, size_t num)
    { return w32_strncpyz((w32_type*)dest, (const w32_type*)src, num); }
#endif

typedef std::vector<w16_type> w16array;
typedef std::vector<w32_type> w32array;

namespace utf8
{
  std::string AnsiToUtf8(const char* ptr);
  std::string Utf8ToAnsi(const char* ptr);

  std::string Utf16ToUtf8(const w16_type* ptr);
  std::string Utf16ToAnsi(const w16_type* ptr);

  std::string Utf32ToUtf8(const w32_type* ptr);

  w16string AnsiToUtf16(const char* ptr);
  w16string Utf8ToUtf16(const char* ptr);
  w32string Utf8ToUtf32(const char* ptr);

  std::wstring Utf8ToWstring(const char* ptr);
  std::string WstringToUtf8(const std::wstring &str);
  std::string WstringToUtf8(const wchar_t* ptr);

  w16string WstringToUtf16(const wchar_t* ptr);
  w16string WstringToUtf16(const std::wstring& str);

  // Returned string can be not terminated by zero if wcslen(ptr) >= limit !!!
  w16array WstringToUtf16(const wchar_t* ptr, size_t limit);
  w16array WstringToUtf16(const std::wstring& str, size_t limit);

  // Returns always terminated by zero string
  w16array WstringToUtf16z(const wchar_t* ptr, size_t limit);
  w16array WstringToUtf16z(const std::wstring& str, size_t limit);

  std::wstring Utf16ToWstring(const w16_type* ptr);
  std::wstring Utf16ToWstring(const w16string& str);

#ifndef _WIN32
  std::string Utf8ToLower(const char* ptr);
  std::string Utf8ToUpper(const char* ptr);
#endif  
}