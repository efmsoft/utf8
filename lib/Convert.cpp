#include <cassert>
#include <cstring>
#include <vector>

#include <utf8/Convert.h>
#include <utf8/StringTemplate.h>

#ifdef _WIN32
  #include <windows.h>
#else 
  #if defined(__APPLE__)
    #include <CoreFoundation/CoreFoundation.h>
  #endif

  #include <iconv.h>
#endif

#define UNI_SUR_HIGH_START  (w32_type)0xD800
#define UNI_SUR_HIGH_END    (w32_type)0xDBFF
#define UNI_SUR_LOW_START   (w32_type)0xDC00
#define UNI_SUR_LOW_END     (w32_type)0xDFFF

#ifndef UNI_MAX_LEGAL_UTF32
#define UNI_MAX_LEGAL_UTF32 (w32_type)0x0010FFFF
#endif

#pragma warning(disable : 5051)

using namespace utf8;

#ifdef _WIN32
    size_t w32_strlen(const w32_type* p) { return xstrlen(p); }
    w32_type* w32_strcpy(w32_type* dest, const w32_type* src) { return xstrcpy(dest, src); }
    w32_type* w32_strncpy(w32_type* dest, const w32_type* src, size_t num) { return xstrncpy(dest, src, num); }
    w32_type* w32_strncpyz(w32_type* dest, const w32_type* src, size_t num) { return xstrncpyz(dest, src, num); }
#else //#if defined(__APPLE__)
    size_t w16_strlen(const w16_type* p) { return xstrlen(p); }
    w16_type* w16_strcpy(w16_type* dest, const w16_type* src) { return xstrcpy(dest, src); }
    w16_type* w16_strncpy(w16_type* dest, const w16_type* src, size_t num) { return xstrncpy(dest, src, num); }
#endif

w16_type* w16_strncpyz(w16_type* dest, const w16_type* src, size_t num) { return xstrncpyz(dest, src, num); }

#ifndef _WIN32
template<typename tstring, typename tchar>
tstring posixEncodeString(
  const char* ptr
  , size_t cbin
  , const char* from
  , const char* to
)
{
  if (cbin == 0)
    return tstring();

  iconv_t h = iconv_open(to, from);
  if (h == (iconv_t)-1)
  {
    assert(!"iconv_open failed");
    return tstring();
  }

  char* in = (char*)ptr;
  size_t cbout = 4 * cbin; // Should be enough in all cases

  char* buffer = (char*)alloca(cbout);
  memset(buffer, 0, cbout);

  char* out = buffer;
  size_t nc = iconv(h, &in, &cbin, &out, &cbout);
  if (nc == -1)
  {
    assert(!"Conversion failed!!");
    *buffer = '\0';
  }

  iconv_close(h);
  return tstring((tchar*)buffer);
}
#endif // #if defined(__APPLE__)

std::wstring utf8::Utf8ToWstring(const char* ptr)
{
#ifdef _WIN32
  return Utf8ToUtf16(ptr);
#else //#if defined(__APPLE__)
  return Utf8ToUtf32(ptr);
#endif
}

std::string utf8::WstringToUtf8(const std::wstring &str)
{
  return utf8::WstringToUtf8(str.c_str());
}

std::string utf8::WstringToUtf8(const wchar_t* ptr)
{
#ifdef _WIN32
  return Utf16ToUtf8(ptr);
#else //#if defined(__APPLE__)
  return Utf32ToUtf8(ptr);
#endif
}

w16string utf8::WstringToUtf16(const wchar_t* ptr)
{
#ifdef _WIN32
  return ptr;
#else //#if defined(__APPLE__)
  std::string utf8 = Utf32ToUtf8(ptr);
  return Utf8ToUtf16(utf8.c_str());
#endif
}

w16string utf8::WstringToUtf16(const std::wstring& str)
{
  return WstringToUtf16(str.c_str());
}

w16array utf8::WstringToUtf16(const wchar_t* ptr, size_t limit)
{
  w16array arr(limit);
#ifdef _WIN32
  w16_strncpy(&arr[0], ptr, limit);
#else //#if defined(__APPLE__)
  std::string utf8 = Utf32ToUtf8(ptr);
  auto str = Utf8ToUtf16(utf8.c_str());
  w16_strncpy(&arr[0], str.c_str(), limit);
#endif
  return arr;
}

w16array utf8::WstringToUtf16(const std::wstring& str, size_t limit)
{
  return WstringToUtf16(str.c_str(), limit);
}

w16array utf8::WstringToUtf16z(const wchar_t* ptr, size_t limit)
{
  w16array arr(limit);
#ifdef _WIN32
  w16_strncpyz(&arr[0], ptr, limit);
#else //#if defined(__APPLE__)
  std::string utf8 = Utf32ToUtf8(ptr);
  auto str = Utf8ToUtf16(utf8.c_str());
  w16_strncpyz(&arr[0], str.c_str(), limit);
#endif
  return arr;
}

w16array utf8::WstringToUtf16z(const std::wstring& str, size_t limit)
{
  return WstringToUtf16z(str.c_str(), limit);
}

std::wstring utf8::Utf16ToWstring(const w16_type* ptr)
{
#ifdef _WIN32
  return ptr;
#else
  std::string utf8 = Utf16ToUtf8(ptr);
  return Utf8ToWstring(utf8.c_str());
#endif
}

std::wstring utf8::Utf16ToWstring(const w16string& str)
{
  return Utf16ToWstring(str.c_str());
}

std::string utf8::AnsiToUtf8(const char* ptr)
{
  w16string w16 = AnsiToUtf16(ptr);
  return Utf16ToUtf8(w16.c_str());
}

std::string utf8::Utf8ToAnsi(const char* ptr)
{
  w16string w16 = Utf8ToUtf16(ptr);
  return Utf16ToAnsi(w16.c_str());
}

w16string utf8::AnsiToUtf16(const char* ptr)
{
#ifdef _WIN32  
  size_t len = strlen(ptr);

  int nRequired = MultiByteToWideChar(
    CP_ACP
    , 0
    , ptr
    , (int)len
    , nullptr
    , 0
  );

  if (nRequired == 0)
    return w16string();

  std::vector<wchar_t> buffer(size_t(nRequired) + 1);
  nRequired = MultiByteToWideChar(
    CP_ACP
    , 0
    , ptr
    , (int)len
    , &buffer[0]
    , nRequired
  );

  if (nRequired == 0)
    return w16string();

  return w16string(&buffer[0]);
#else
  return posixEncodeString<w16string, w16_type>(
    ptr
    , strlen(ptr)
    , "WINDOWS-1251"
    , "UTF-16LE"
  );
#endif  
}

std::string utf8::Utf16ToUtf8(const w16_type* ptr)
{
#ifdef _WIN32
  size_t len = wcslen(ptr);

  int nRequired = WideCharToMultiByte(
    CP_UTF8
    , 0
    , ptr
    , (int)len
    , nullptr
    , 0
    , nullptr
    , nullptr
  );

  if (nRequired == 0)
    return std::string();

  std::vector<char> buffer(size_t(nRequired) + 1);
  nRequired = WideCharToMultiByte(
    CP_UTF8
    , 0
    , ptr
    , (int)len
    , &buffer[0]
    , nRequired
    , nullptr
    , nullptr
  );

  if (nRequired == 0)
    return std::string();

  return std::string(&buffer[0]);
#else
  return posixEncodeString<std::string, char>(
    (const char*)ptr
    , w16_strlen((const char16_t*)ptr) * sizeof(w16_type)
    , "UTF-16LE"
    , "UTF-8"
  );
#endif  
}

std::string utf8::Utf16ToAnsi(const w16_type* ptr)
{
#ifdef _WIN32
  size_t len = wcslen(ptr);

  int nRequired = WideCharToMultiByte(
    CP_ACP
    , 0
    , ptr
    , (int)len
    , nullptr
    , 0
    , nullptr
    , nullptr
  );

  if (nRequired == 0)
    return std::string();

  std::vector<char> buffer(size_t(nRequired) + 1);
  nRequired = WideCharToMultiByte(
    CP_ACP
    , 0
    , ptr
    , (int)len
    , &buffer[0]
    , nRequired
    , nullptr
    , nullptr
  );

  if (nRequired == 0)
    return std::string();

  return std::string(&buffer[0]);
#else
  return posixEncodeString<std::string, char>(
    (const char*)ptr
    , w16_strlen((const char16_t*)ptr) * sizeof(w16_type)
    , "UTF-16LE"
    , "WINDOWS-1251"
  );
#endif  
}

w16string utf8::Utf8ToUtf16(const char* ptr)
{
#ifdef _WIN32
  size_t len = strlen(ptr);

  int nRequired = MultiByteToWideChar(
    CP_UTF8
    , 0
    , ptr
    , (int)len
    , nullptr
    , 0
  );

  if (nRequired == 0)
    return w16string();

  std::vector<w16_type> buffer(size_t(nRequired) + 1);
  nRequired = MultiByteToWideChar(
    CP_UTF8
    , 0
    , ptr
    , (int)len
    , &buffer[0]
    , nRequired
  );

  if (nRequired == 0)
    return w16string();

  return w16string(&buffer[0]);
#else
  return posixEncodeString<w16string, w16_type>(
    ptr
    , strlen(ptr)
    , "UTF-8"
    , "UTF-16LE"
  );
#endif  
}

inline int is_surrogate(w16_type uc) 
{ 
  return (uc - 0xd800u) < 2048u; 
}

inline int is_high_surrogate(w16_type uc)
{ 
  return (uc & 0xfffffc00) == 0xd800; 
}

inline int is_low_surrogate(w16_type uc)
{ 
  return (uc & 0xfffffc00) == 0xdc00; 
}

inline w32_type surrogate_to_utf32(w16_type high, w16_type low)
{
  return (high << 10) + low - 0x35fdc00;
}

w32string utf8::Utf8ToUtf32(const char* ptr)
{
  w16string w16 = Utf8ToUtf16(ptr);
  size_t size = w16.size();

  w32string w32;
  w32.reserve(size);

  for (size_t i = 0; i < size; ++i)
  {
    w16_type ch = w16[i];
    if (!is_surrogate(ch))
    {
      w32.push_back(ch);
      continue;
    }
    
    if (is_high_surrogate(ch))
    {
      if (i + 1 < w16.size())
      {
        w16_type ch2 = w16[++i];
        if (is_low_surrogate(ch2))
        {
          w32.push_back(surrogate_to_utf32(ch, ch2));
          continue;
        }
      }
    }

    assert(!"Invalid utf-16 string");
    return w32string();
  }
  return w32;
}

/*
 * Once the bits are split out into bytes of UTF-8, this is a mask OR-ed
 * into the first byte, depending on how many bytes follow.  There are
 * as many entries in this table as there are UTF-8 sequence types.
 * (I.e., one byte sequence, two byte... etc.). Remember that sequencs
 * for *legal* UTF-8 will be 4 or fewer bytes total.
 */
static const w32_type FirstByteMark[7] = 
{ 
  0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC 
};

std::string utf8::Utf32ToUtf8(const w32_type* source)
{
  std::string utf8;

  while (*source) 
  {
    // UTF-16 surrogate values are illegal in UTF-32
    w32_type ch = *source++;
    if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) 
      return std::string();

    // Figure out how many bytes the result will require. Turn any
    // illegally large UTF32 things (> Plane 17) into replacement chars.
    int cb = 0;
    if (ch < (w32_type)0x80)
      cb = 1;
    else if (ch < (w32_type)0x800) 
      cb = 2;
    else if (ch < (w32_type)0x10000) 
      cb = 3;
    else if (ch <= UNI_MAX_LEGAL_UTF32) 
      cb = 4;
    else 
      return std::string();

    const w32_type byteMask = 0xBF;
    const w32_type byteMark = 0x80;

    char arr[4]{};
    char* p = arr + cb - 1;

    switch (cb) 
    {
    case 4: *p-- = (char)((ch | byteMark) & byteMask); ch >>= 6; [[fallthrough]];
    case 3: *p-- = (char)((ch | byteMark) & byteMask); ch >>= 6; [[fallthrough]];
    case 2: *p-- = (char)((ch | byteMark) & byteMask); ch >>= 6; [[fallthrough]];
    case 1: *p = (char)(ch | FirstByteMark[cb]);
    }

    for (int i = 0; i < cb; ++i)
      utf8.push_back(arr[i]);
  }
  return utf8;
}

#ifdef __APPLE__
std::string utf8::Utf8ToLower(const char* ptr)
{
  CFStringRef hs = CFStringCreateWithCString(nullptr, ptr, kCFStringEncodingUTF8);
  if (hs == nullptr)
    return std::string();

  CFMutableStringRef h = CFStringCreateMutableCopy(
    nullptr
    , 0
    , hs
  );
  CFRelease(hs);

  if (h == nullptr)
    return std::string();

  CFStringLowercase(h, NULL);

  CFIndex sizeChar = CFStringGetLength(h)+1;
  char * strChar = (char *) malloc(sizeChar);
  bool success = CFStringGetCString(h, strChar, sizeChar, kCFStringEncodingUTF8);
  CFRelease(h);

  std::string result(strChar);
  delete strChar;

  if (success)
  {
    return result;
  }
  return std::string();
}

std::string utf8::Utf8ToUpper(const char* ptr)
{
  CFStringRef hs = CFStringCreateWithCString(nullptr, ptr, kCFStringEncodingUTF8); 
  if (hs == nullptr)
    return std::string();

  CFMutableStringRef h = CFStringCreateMutableCopy(
    nullptr
    , 0
    , hs
  );
  CFRelease(hs);

  if (h == nullptr)
    return std::string();

  CFStringUppercase(h, NULL);

  CFIndex sizeChar = CFStringGetLength(h)+1;
  char * strChar = (char *) malloc(sizeChar);
  bool success = CFStringGetCString(h, strChar, sizeChar, kCFStringEncodingUTF8);
  CFRelease(h);

  std::string result(strChar);
  delete strChar;

  if (success)
  {
    return result;
  }
  return std::string();
}
#endif  
