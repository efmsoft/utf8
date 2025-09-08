#include <algorithm>
#include <cassert>
#include <clocale>
#include <cstring>
#include <cwctype>
#include <iomanip>
#include <locale>
#include <sstream>

#include <utf8/Convert.h>
#include <utf8/String.h>

#ifdef UTF8_STATIC_WINDOWS_RUNTIME
  #define SUPPRESS_LEGACY_ICU_HEADER_WARNINGS
  #include <icucommon.h>
  #include <icui18n.h>
#else
  #include <unicode/uchar.h>
#endif

using namespace utf8;

String::String()
{
}

String::String(const String& str)
{
  Data = str.Data;
}

String::String(String&& str) noexcept
  : Data(std::move(str.Data))
{
}

String::String(const Char& ch, size_t n)
{
  std::string str = ch.Str();
  ASSERT_VALID_UTF8(str);

  for (size_t i = 0; i < n; ++i)
    Data += str;
}

String::String(const AnsiPtr& ptr)
{
  Data = AnsiToUtf8(ptr);
  ASSERT_VALID_UTF8(Data);
}

String::String(const Utf8Ptr& ptr)
  : Data(ptr)
{
  ASSERT_VALID_UTF8(Data);
}

String::String(const w16string& str)
{
  Data = Utf16ToUtf8(str.c_str());
  ASSERT_VALID_UTF8(Data);
}

String::String(const w16_type* ptr, size_t n)
{
  w16string str(ptr, n == -1 ? w16_strlen(ptr) : n);
  Data = Utf16ToUtf8(str.c_str());
  ASSERT_VALID_UTF8(Data);
}

String::String(const w32string& str)
{
  Data = Utf32ToUtf8(str.c_str());
  ASSERT_VALID_UTF8(Data);
}

String::String(const w32_type* ptr, size_t n)
{
  w32string str(ptr, n == -1 ? w32_strlen(ptr) : n);
  Data = Utf32ToUtf8(str.c_str());
  ASSERT_VALID_UTF8(Data);
}

String::String(const char* utf8, size_t n)
  : Data(utf8, n == -1 ? strlen(utf8) : n)
{
  ASSERT_VALID_UTF8(Data);
}

String::String(const std::string& utf8)
  : Data(utf8)
{
  ASSERT_VALID_UTF8(Data);
}

#ifdef AK_WXWIDGETS_ON
String::String(const wxString& string)
{
    Data = std::string(string.mb_str(wxConvUTF8));
    ASSERT_VALID_UTF8(Data);
}
#endif

const char* String::c_str() const
{
  return Data.c_str();
}

String::operator const char* () const
{
  return Data.c_str();
}

String::operator const std::string& () const
{
  return Data;
}

const std::string& String::Str() const
{
  return Data;
}

std::string String::ToAnsi() const
{
  return Utf8ToAnsi(Data.c_str());
}

std::string String::ToUtf8() const
{
  return Data;
}

std::string String::ToNativeString() const
{
#ifdef _WIN32
  return ToAnsi();
#else
  return Data;
#endif
}

std::wstring String::ToWstring() const
{
  return Utf8ToWstring(Data.c_str());
}

#ifdef AK_WXWIDGETS_ON

wxString String::ToWxString() const
{
  return wxString::FromUTF8(Data);
}
#endif

size_t String::CharSize(const char* p)
{
  if (p == nullptr) 
    return 0;

  char ch = *p;
  if (ch == 0) 
    return 0;

  // See Encoding (https://en.wikipedia.org/wiki/UTF-8)

  if ((ch & 0x80) == 0) 
    return 1;
  else if ((ch & 0xE0) == 0xC0) 
    return 2;
  else if ((ch & 0xF0) == 0xE0) 
    return 3;
  else if ((ch & 0xF8) == 0xF0) 
    return 4;

  assert(!"Invalid utf-8 string");
  return 0;
}

bool String::Empty() const
{
  return Length() == 0;
}

size_t String::Length() const
{
  size_t len = 0;
  size_t bytes = 0;
  size_t size = Data.size();
  for (const char* p = Data.c_str(); *p;)
  {
    size_t n = CharSize(p);
    if (!n)
      break;

    len++;
    p += n;
    bytes += n;

    if (bytes > size)
      return 0;
  }
  return len;
}

size_t String::Size() const
{
  return Data.size();
}

size_t String::SizeOf(size_t charIndex) const
{
  for (const char* p = Data.c_str(); *p;)
  {
    size_t n = CharSize(p);
    if (!n)
      break;

    if (!charIndex--)
      return n;

    p += n;
  }
  return 0;
}

size_t String::OffsetOf(size_t charIndex) const
{
  size_t offset = 0;
  for (const char* p = Data.c_str(); *p;)
  {
    size_t n = CharSize(p);
    if (!n)
      break;

    if (!charIndex--)
      return offset;

    p += n;
    offset += n;
  }
  return 0;
}

Char String::CharAt(size_t charIndex) const
{
  Char ch;

  size_t size = SizeOf(charIndex);
  for (const char* p = &Data[0] + OffsetOf(charIndex); size; --size)
    ch.push_back(*p++);

  return ch;
}

Char String::operator[](size_t charIndex) const
{
  return CharAt(charIndex);
}

Char String::LastChar() const
{
  if (Empty())
    return Char();

  return CharAt(Length() - 1);
}

bool String::RemoveAt(size_t charIndex)
{
  size_t len = Length();
  if (len <= charIndex)
    return false;

  size_t pos = OffsetOf(charIndex);
  size_t cb = CharSize(c_str() + pos);

  Data = Data.substr(0, pos) + Data.substr(pos + cb);
  return true;
}

bool String::InsertAt(size_t charIndex, const Char& ch)
{
  size_t len = Length();
  if (charIndex == std::string::npos)
    charIndex = len;

  if (len < charIndex)
    return false;

  size_t pos;
  if (charIndex == len)
    pos = Data.size();
  else
     pos = OffsetOf(charIndex);
     
  Data = Data.substr(0, pos) + ch.Str() + Data.substr(pos);
  return true;
}

bool String::ReplaceAt(size_t charIndex, const Char& ch)
{
  size_t len = Length();
  if (len <= charIndex)
    return false;

  size_t pos = OffsetOf(charIndex);
  size_t cb = CharSize(c_str() + pos);
  Data = Data.substr(0, pos) + ch.Str() + Data.substr(pos + cb);
  return true;
}

void String::Clear()
{
  Data.clear();
}

void String::ToLowerCase()
{
#ifdef _WIN32
  w32string src = Utf8ToUtf32(Data.c_str());
  w32string dst;

  for (auto c : src)
    dst += u_tolower(c);

  Data = Utf32ToUtf8(dst.c_str());
#elif defined(__APPLE__)
  Data = Utf8ToLower(Data.c_str());
#endif
}

void String::ToUpperCase()
{
#ifdef _WIN32
  w32string src = Utf8ToUtf32(Data.c_str());
  w32string dst;

  for (auto c : src)
    dst += u_toupper(c);

  Data = Utf32ToUtf8(dst.c_str());
#elif defined(__APPLE__)
  Data = Utf8ToUpper(Data.c_str());
#endif
}

void String::Trim(const Char& space)
{
  TrimLeft(space);
  TrimRight(space);
}

void String::TrimLeft(const Char& space)
{
  size_t len = Length();
  for (size_t i = 0; i < len; ++i)
  {
    if (CharAt(i) == space)
      continue;

    if (i)
      Data = Data.substr(OffsetOf(i));

    return;
  }

  Data.clear();
}

void String::TrimRight(const Char& space)
{
  if (Empty())
    return;

  size_t len = Length();
  for (int i = int(len - 1); i >= 0; --i)
  {
    if (CharAt(i) == space)
      continue;

    if (i != int(len - 1))
      Data = Data.substr(0, OffsetOf(size_t(i) + 1));

    return;
  }

  Data.clear();
}

void String::Remove(const Char& ch)
{
  char* p = &Data[0];
  size_t size = Data.size();
  size_t cb = size + 1;

  Char c;
  c.reserve(3);

  for (;;)
  {
    size_t n = CharSize(p);
    if (!n)
      break;

    c.clear();
    for (size_t i = 0; i < n; ++i)
      c.push_back(p[i]);

    if (c != ch)
    {
      p += n;
      cb -= n;
      continue;
    }

    cb -= n;
    size -= n;
    memmove(p, p + n, cb);
  }

  Data.resize(size);
}

void String::Replace(const Char& find, const Char& replace)
{
  if (find == replace)
    return;

  while (true)
  {
    size_t index = IndexOf(find);
    if (index == std::string::npos)
      break;

    ReplaceAt(index, replace);
  }
}

bool String::ReplaceString(const String& find, const String& replace)
{
  bool result = false;
  size_t startPos = 0;
  size_t newStartPos = 0;
  while ((newStartPos = IndexOf(find, startPos)) != std::string::npos)
  {
    *this = Substr(0, newStartPos) + replace + Substr(newStartPos + find.Length());
    startPos = newStartPos + replace.Length();
    result = true;
  }

  return result;
}

String String::Substr(size_t pos, size_t count) const
{
  size_t len = Length();
  if (pos >= len)
    return String();

  std::string str;

  size_t end = pos + count;
  for (const char* p = c_str() + OffsetOf(pos); pos < len && *p; ++pos)
  {
    if (count != std::string::npos)
    {
      if (pos == end)
        break;
    }

    size_t n = CharSize(p);
    if (!n)
      break;

    for (size_t i = 0; i < n; ++i)
      str.push_back(*p++);
  }

  ASSERT_VALID_UTF8(str);
  return String(Utf8Ptr(str));
}

StringArray String::Split(const char* delimiters) const
{
  CharSet chs;

  size_t size = strlen(delimiters);
  for (size_t offset = 0; offset < size;)
  {
    size_t cb = CharSize(delimiters);
    if (!cb || offset + cb > size)
      break;

    Char ch;
    for (size_t i = 0; i < cb; ++i)
      ch.push_back(delimiters[i]);

    chs.insert(ch);

    delimiters += cb;
    offset += cb;
  }

  return Split(chs);
}

StringArray String::Split(const CharSet& delimiters) const
{
  StringArray tokens;

  if (Empty())
      return tokens;

  const char* start = nullptr;
  const char* p = Data.c_str();

  Char ch;
  ch.reserve(3);

  for (;;)
  {
    size_t n = CharSize(p);
    if (!n)
      break;

    ch.clear();
    for (size_t i = 0; i < n; ++i)
      ch.push_back(p[i]);

    auto r = delimiters.find(ch);
    if (r == delimiters.end())
    {
      if (start == nullptr)
        start = p;

      p += n;
      continue;
    }
    
    if (start == nullptr)
      tokens.push_back(String());
    else
      tokens.push_back(String(start, p - start));

    p += n;
    start = nullptr;
  }

  if (start)
    tokens.push_back(String(start, p - start));
  else
    tokens.push_back(String());

  return tokens;
}

String String::Join(
  const StringArray& arr
  , const Char& delimiter
)
{
  String str;

  int i = 0;
  for (auto& s : arr)
  {
    if (i++)
      str += delimiter;

    str += s;
  }
  return str;
}

bool String::StartsWith(const String& str) const
{
  return Data.find(str.Data) == 0;
}

bool String::StartsWith(const AnsiPtr& ptr) const
{
  String utf8(ptr);
  return StartsWith(utf8);
}

bool String::StartsWith(const Utf8Ptr& ptr) const
{
  String utf8(ptr);
  return StartsWith(utf8);
}

bool String::StartsWith(const w16string& str) const
{
  String utf8(str);
  return StartsWith(utf8);
}

bool String::StartsWith(const w16_type* ptr) const
{
  String utf8(ptr);
  return StartsWith(utf8);
}

bool String::StartsWith(const w32string& str) const
{
  String utf8(str);
  return StartsWith(utf8);
}

bool String::StartsWith(const w32_type* ptr) const
{
  String utf8(ptr);
  return StartsWith(utf8);
}

bool String::StartsWith(const Char& ch) const
{
  String utf8(ch, 1);
  return StartsWith(utf8);
}

bool String::StartsWith(const char* ptr) const
{
  String utf8(ptr);
  return StartsWith(utf8);
}

bool String::StartsWith(const std::string& str) const
{
  String utf8(str);
  return StartsWith(utf8);
}

bool String::EndsWith(const String& str) const
{
  size_t l0 = Length();
  size_t l1 = str.Length();
  if (l1 > l0)
    return false;

  return Data.rfind(str.Data) == l0 - l1;
}

bool String::EndsWith(const AnsiPtr& ptr) const
{
  String utf8(ptr);
  return EndsWith(utf8);
}

bool String::EndsWith(const Utf8Ptr& ptr) const
{
  String utf8(ptr);
  return EndsWith(utf8);
}

bool String::EndsWith(const w16string& str) const
{
  String utf8(str);
  return EndsWith(utf8);
}

bool String::EndsWith(const w16_type* ptr) const
{
  String utf8(ptr);
  return EndsWith(utf8);
}

bool String::EndsWith(const w32string& str) const
{
  String utf8(str);
  return EndsWith(utf8);
}

bool String::EndsWith(const w32_type* ptr) const
{
  String utf8(ptr);
  return EndsWith(utf8);
}

bool String::EndsWith(const Char& ch) const
{
  String utf8(ch, 1);
  return EndsWith(utf8);
}

bool String::EndsWith(const char* ptr) const
{
  String utf8(ptr);
  return EndsWith(utf8);
}

bool String::EndsWith(const std::string& str) const
{
  String utf8(str);
  return EndsWith(utf8);
}

bool String::Includes(const String& str, size_t pos) const
{
  size_t len = Length();
  if (pos >= len)
    return false;

  const char* p0 = c_str() + OffsetOf(pos);
  return strstr(p0, str.Data.c_str()) != nullptr;
}

bool String::Includes(const AnsiPtr& ptr, size_t pos) const
{
  String utf8(ptr);
  return Includes(utf8, pos);
}

bool String::Includes(const Utf8Ptr& ptr, size_t pos) const
{
  String utf8(ptr);
  return Includes(utf8, pos);
}

bool String::Includes(const w16string& str, size_t pos) const
{
  String utf8(str);
  return Includes(utf8, pos);
}

bool String::Includes(const w16_type* ptr, size_t pos) const
{
  String utf8(ptr);
  return Includes(utf8, pos);
}

bool String::Includes(const w32string& str, size_t pos) const
{
  String utf8(str);
  return Includes(utf8, pos);
}

bool String::Includes(const w32_type* ptr, size_t pos) const
{
  String utf8(ptr);
  return Includes(utf8, pos);
}

bool String::Includes(const Char& ch, size_t pos) const
{
  String utf8(ch, 1);
  return Includes(utf8, pos);
}

bool String::Includes(const char* ptr, size_t pos) const
{
  String utf8(ptr);
  return Includes(utf8, pos);
}

bool String::Includes(const std::string& str, size_t pos) const
{
  String utf8(str);
  return Includes(utf8, pos);
}

size_t String::PtrToPos(const char* p0) const
{
  size_t index = 0;
  size_t bytes = 0;
  size_t size = Data.size();

  for (const char* p = Data.c_str(); *p;)
  {
    if (p == p0)
      return index;

    size_t n = CharSize(p);
    if (!n)
      return std::string::npos;

    index++;
    bytes += n;
    p += n;

    if (bytes > size)
      return std::string::npos;
  }
  return std::string::npos;
}

size_t String::PosToBitPos(const size_t& pos) const
{
  size_t index = 0;
  size_t bytes = 0;
  size_t size = Data.size();

  for (const char* p = Data.c_str(); *p;)
  {
    if (index >= pos)
      break;

    size_t n = CharSize(p);
    if (!n)
      return std::string::npos;

    index++;
    bytes += n;
    p += n;

    if (bytes > size)
      return std::string::npos;
  }
  return bytes;
}

size_t String::IndexOf(const String& str, size_t Off) const
{
  size_t pos = Data.find(str.Data, PosToBitPos(Off));

  if (pos == std::string::npos)
    return std::string::npos;
  
  return PtrToPos(c_str() + pos);
}

size_t String::IndexOf(const AnsiPtr& ptr, size_t Off) const
{
  String utf8(ptr);
  return IndexOf(utf8, Off);
}

size_t String::IndexOf(const Utf8Ptr& ptr, size_t Off) const
{
  String utf8(ptr);
  return IndexOf(utf8, Off);
}

size_t String::IndexOf(const w16string& str, size_t Off) const
{
  String utf8(str);
  return IndexOf(utf8, Off);
}

size_t String::IndexOf(const w16_type* ptr, size_t Off) const
{
  String utf8(ptr);
  return IndexOf(utf8, Off);
}

size_t String::IndexOf(const w32string& str, size_t Off) const
{
  String utf8(str);
  return IndexOf(utf8, Off);
}

size_t String::IndexOf(const w32_type* ptr, size_t Off) const
{
  String utf8(ptr);
  return IndexOf(utf8, Off);
}

size_t String::IndexOf(const Char& ch, size_t Off) const
{
  String utf8(ch, 1);
  return IndexOf(utf8, Off);
}

size_t String::IndexOf(const char* ptr, size_t Off) const
{
  String utf8(ptr);
  return IndexOf(utf8, Off);
}

size_t String::IndexOf(const std::string& str, size_t Off) const
{
  String utf8(str);
  return IndexOf(utf8, Off);
}

size_t String::LastIndexOf(const String& str) const
{
  size_t pos = Data.rfind(str.Data);
  if (pos == std::string::npos)
    return std::string::npos;

  return PtrToPos(c_str() + pos);
}

size_t String::LastIndexOf(const AnsiPtr& ptr) const
{
  String utf8(ptr);
  return LastIndexOf(utf8);
}

size_t String::LastIndexOf(const Utf8Ptr& ptr) const
{
  String utf8(ptr);
  return LastIndexOf(utf8);
}

size_t String::LastIndexOf(const w16string& str) const
{
  String utf8(str);
  return LastIndexOf(utf8);
}

size_t String::LastIndexOf(const w16_type* ptr) const
{
  String utf8(ptr);
  return LastIndexOf(utf8);
}

size_t String::LastIndexOf(const w32string& str) const
{
  String utf8(str);
  return LastIndexOf(utf8);
}

size_t String::LastIndexOf(const w32_type* ptr) const
{
  String utf8(ptr);
  return LastIndexOf(utf8);
}

size_t String::LastIndexOf(const Char& ch) const
{
  String utf8(ch, 1);
  return LastIndexOf(utf8);
}

size_t String::LastIndexOf(const char* ptr) const
{
  String utf8(ptr);
  return LastIndexOf(utf8);
}

size_t String::LastIndexOf(const std::string& str) const
{
  String utf8(str);
  return LastIndexOf(utf8);
}

bool String::IsEqual(const String& str) const
{
  return operator==(str);
}

bool String::IsEqual(const AnsiPtr& ptr) const
{
  return operator==(ptr);
}

bool String::IsEqual(const Utf8Ptr& ptr) const
{
  return operator==(ptr);
}

bool String::IsEqual(const w16string& str) const
{
  return operator==(str);
}

bool String::IsEqual(const w16_type* ptr) const
{
  return operator==(ptr);
}

bool String::IsEqual(const w32string& str) const
{
  return operator==(str);
}

bool String::IsEqual(const w32_type* ptr) const
{
  return operator==(ptr);
}

bool String::IsEqual(const Char& ch) const
{
  return operator==(ch);
}

bool String::IsEqual(const char* ptr) const
{
  String utf8(ptr);
  return operator==(utf8);
}

bool String::IsEqual(const std::string& str) const
{
  String utf8(str);
  return operator==(utf8);
}

bool String::IsEqualNoCase(const String& str) const
{
  String s1(*this);
  String s2(str);

  s1.ToLowerCase();
  s2.ToLowerCase();

  return s1 == s2;
}

bool String::IsEqualNoCase(const AnsiPtr& ptr) const
{
  String utf8(ptr);
  return IsEqualNoCase(utf8);
}

bool String::IsEqualNoCase(const Utf8Ptr& ptr) const
{
  String utf8(ptr);
  return IsEqualNoCase(utf8);
}

bool String::IsEqualNoCase(const w16string& str) const
{
  String utf8(str);
  return IsEqualNoCase(utf8);
}

bool String::IsEqualNoCase(const w16_type* ptr) const
{
  String utf8(ptr);
  return IsEqualNoCase(utf8);
}

bool String::IsEqualNoCase(const w32string& str) const
{
  String utf8(str);
  return IsEqualNoCase(utf8);
}

bool String::IsEqualNoCase(const w32_type* ptr) const
{
  String utf8(ptr);
  return IsEqualNoCase(utf8);
}

bool String::IsEqualNoCase(const Char& ch) const
{
  String utf8(ch, 1);
  return IsEqualNoCase(utf8);
}

bool String::IsEqualNoCase(const char* ptr) const
{
  String utf8(ptr);
  return IsEqualNoCase(utf8);
}

bool String::IsEqualNoCase(const std::string& str) const
{
  String utf8(str);
  return IsEqualNoCase(utf8);
}

bool String::operator==(const String& str) const
{
  return Data == str.Data;
}

bool String::operator==(const AnsiPtr& ptr) const
{
  String str(ptr);
  return Data == str.Data;
}

bool String::operator==(const Utf8Ptr& ptr) const
{
  String str(ptr);
  return Data == str.Data;
}

bool String::operator==(const w16string& str) const
{
  String utf8(str);
  return Data == utf8.Data;
}

bool String::operator==(const w16_type* ptr) const
{
  String utf8(ptr);
  return Data == utf8.Data;
}

bool String::operator==(const w32string& str) const
{
  String utf8(str);
  return Data == utf8.Data;
}

bool String::operator==(const w32_type* ptr) const
{
  String utf8(ptr);
  return Data == utf8.Data;
}

bool String::operator==(const Char& ch) const
{
  String utf8(ch, 1);
  return Data == utf8.Data;
}

bool String::operator==(const char* ptr) const
{
  return Data == ptr;
}

bool String::operator==(const std::string& str) const
{
  return Data == str;
}

bool String::operator!=(const String& str) const
{
  return !operator==(str);
}

bool String::operator!=(const AnsiPtr& ptr) const
{
  return !operator==(ptr);
}

bool String::operator!=(const Utf8Ptr& ptr) const
{
  return !operator==(ptr);
}

bool String::operator!=(const w16string& str) const
{
  return !operator==(str);
}

bool String::operator!=(const w16_type* ptr) const
{
  return !operator==(ptr);
}

bool String::operator!=(const w32string& str) const
{
  return !operator==(str);
}

bool String::operator!=(const w32_type* ptr) const
{
  return !operator==(ptr);
}

bool String::operator!=(const Char& ch) const
{
  return !operator==(ch);
}

bool String::operator!=(const char* ptr) const
{
  return !operator==(ptr);
}

bool String::operator!=(const std::string& str) const
{
  return !operator==(str);
}

String& String::operator=(const String& str)
{
  Data = str.ToUtf8();
  return *this;
}

String& String::operator=(const AnsiPtr& ptr)
{
  Data = AnsiToUtf8(ptr);
  return *this;
}

String& String::operator=(const Utf8Ptr& ptr)
{
  Data = ptr;
  return *this;
}

String& String::operator=(const w16string& str)
{
  Data = Utf16ToUtf8(str.c_str());
  return *this;
}

String& String::operator=(const w16_type* ptr)
{
  Data = Utf16ToUtf8(ptr);
  return *this;
}

String& String::operator=(const w32string& str)
{
  Data = Utf32ToUtf8(str.c_str());
  return *this;
}

String& String::operator=(const w32_type* ptr)
{
  Data = Utf32ToUtf8(ptr);
  return *this;
}

String& String::operator=(const Char& ch)
{
  Data = ch.Str();
  return *this;
}

String& String::operator=(const std::string& str)
{
  ASSERT_VALID_UTF8(str);

  Data = str;
  return *this;
}

String& String::operator=(const char* ptr)
{
  ASSERT_VALID_UTF8(ptr);

  Data = ptr;
  return *this;
}

String& String::operator+=(const String& str)
{
  Data += str.c_str();
  return *this;
}

String& String::operator+=(const AnsiPtr& ptr)
{
  Data += AnsiToUtf8(ptr);
  return *this;
}

String& String::operator+=(const Utf8Ptr& ptr)
{
  Data += ptr;
  return *this;
}

String& String::operator+=(const w16string& str)
{
  Data += Utf16ToUtf8(str.c_str());
  return *this;
}

String& String::operator+=(const w16_type* ptr)
{
  Data += Utf16ToUtf8(ptr);
  return *this;
}

String& String::operator+=(const w32string& str)
{
  Data += Utf32ToUtf8(str.c_str());
  return *this;
}

String& String::operator+=(const w32_type* ptr)
{
  Data += Utf32ToUtf8(ptr);
  return *this;
}

String& String::operator+=(const Char& ch)
{
  Data += ch.Str();
  return *this;
}

String& String::operator+=(const std::string& str)
{
  ASSERT_VALID_UTF8(str);

  Data += str;
  return *this;
}

String& String::operator+=(const char* ptr)
{
  ASSERT_VALID_UTF8(ptr);

  Data += ptr;
  return *this;
}

String String::operator+(const String& str) const
{
  std::string data = Data + str.ToUtf8();
  return String(Utf8Ptr(data));
}

String String::operator+(const AnsiPtr& ptr) const
{
  std::string data = Data + AnsiToUtf8(ptr);
  return String(Utf8Ptr(data));
}

String String::operator+(const Utf8Ptr& ptr) const
{
  std::string data = Data + *ptr;
  return String(Utf8Ptr(data));
}

String String::operator+(const w16string& str) const
{
  std::string data = Data + Utf16ToUtf8(str.c_str());
  return String(Utf8Ptr(data));
}

String String::operator+(const w16_type* ptr) const
{
  std::string data = Data + Utf16ToUtf8(ptr);
  return String(Utf8Ptr(data));
}

String String::operator+(const w32string& str) const
{
  std::string data = Data + Utf32ToUtf8(str.c_str());
  return String(Utf8Ptr(data));
}

String String::operator+(const w32_type* ptr) const
{
  std::string data = Data + Utf32ToUtf8(ptr);
  return String(Utf8Ptr(data));
}

String String::operator+(const Char& ch) const
{
  std::string data = Data + ch.Str();
  return String(Utf8Ptr(data));
}

String String::operator+(char ch) const
{
  return *this + Char(ch);
}

String String::operator+(const std::string& str) const
{
  return Data + str;
}

String String::operator+(const char* ptr) const
{
  return Data + ptr;
}

bool String::operator<(const String& str) const
{
  return Data < str.Data;
}

bool String::operator<(const AnsiPtr& ptr) const
{
  String str(ptr);
  return Data < str.Data;
}

bool String::operator<(const Utf8Ptr& ptr) const
{
  String str(ptr);
  return Data < str.Data;
}

bool String::operator<(const w16string& str) const
{
  String utf8(str);
  return Data < utf8.Data;
}

bool String::operator<(const w16_type* ptr) const
{
  String utf8(ptr);
  return Data < utf8.Data;
}

bool String::operator<(const w32string& str) const
{
  String utf8(str);
  return Data < utf8.Data;
}

bool String::operator<(const w32_type * ptr) const
{
  String utf8(ptr);
  return Data < utf8.Data;
}

bool String::operator<(char ch) const
{
  String utf8(ch, 1);
  return Data < utf8.Data;
}

bool String::operator<(const Char& ch) const
{
  String utf8(ch, 1);
  return Data < utf8.Data;
}

bool String::operator<(const char* ptr) const
{
  return Data < ptr;
}

bool String::operator<(const std::string& str) const
{
  return Data < str;
}

bool String::Valid(const std::string& str)
{
  return Valid(str.c_str());
}

bool String::Valid(const char* ptr)
{
  return Verify(ptr) == nullptr;
}

const char* String::Verify(const char* ptr)
{
  unsigned char* s = (unsigned char*)ptr;

  while (*s) 
  {
    if (*s < 0x80)
    {
      s++; // 0xxxxxxx
      continue;
    }

    if ((s[0] & 0xe0) == 0xc0) 
    {
      // 110XXXXx 10xxxxxx
      if ((s[1] & 0xc0) != 0x80 || (s[0] & 0xfe) == 0xc0) // overlong?
        return (const char*)s;
      
      s += 2;
      continue;
    }
    
    if ((s[0] & 0xf0) == 0xe0) 
    {
      // 1110XXXX 10Xxxxxx 10xxxxxx
      if ((s[1] & 0xc0) != 0x80 ||
          (s[2] & 0xc0) != 0x80 ||
          (s[0] == 0xe0 && (s[1] & 0xe0) == 0x80) ||              // overlong?
          (s[0] == 0xed && (s[1] & 0xe0) == 0xa0) ||              // surrogate?
          (s[0] == 0xef && s[1] == 0xbf && (s[2] & 0xfe) == 0xbe) // U+FFFE or U+FFFF?
      )                      
      {
        return (const char*)s;
      }
      s += 3;
      continue;
    } 
    
    if ((s[0] & 0xf8) == 0xf0) 
    {
      // 11110XXX 10XXxxxx 10xxxxxx 10xxxxxx
      if ((s[1] & 0xc0) != 0x80 ||
          (s[2] & 0xc0) != 0x80 ||
          (s[3] & 0xc0) != 0x80 ||
          (s[0] == 0xf0 && (s[1] & 0xf0) == 0x80) ||    // overlong?
          (s[0] == 0xf4 && s[1] > 0x8f) || s[0] > 0xf4   // > U+10FFFF?
      )
      {
        return (const char*)s;
      }
      s += 4;
      continue;
    } 
    return (const char*)s;
  }
  return nullptr;
}

String operator+(const char* left, const String& str) 
{ 
  return String(left) + str; 
}

String operator+(const std::string & left, const String& str)
{
  return String(left) + str;
}