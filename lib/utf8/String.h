#pragma once

#include <utf8/Char.h>
#include <utf8/ConstStringPtr.h>
#include <utf8/Convert.h>

#ifdef _DEBUG
  #define ASSERT_VALID_UTF8(s)   assert(utf8::String::Valid(s))
#else
  #define ASSERT_VALID_UTF8(s)   
#endif

namespace utf8
{
  class String;
  typedef std::vector<String> StringArray;

  class String
  {
    std::string Data;

  public:
    String();
    String(const String& str);
    String(String&& str) noexcept;
    String(const Char& ch, size_t n);
    String(const AnsiPtr& ptr);
    String(const Utf8Ptr& ptr);
    String(const w16string& str);
    String(const w16_type* ptr, size_t n = -1);
    String(const w32string& str);
    String(const w32_type* ptr, size_t n = -1);
    String(const char* utf8, size_t n = -1);
    String(const std::string& utf8);

    // C string pointer / string reference
    const char* c_str() const;
    operator const char* () const;
    operator const std::string& () const;
    const std::string& Str() const;

    // Length and size of string
    bool Empty() const;         // Length() == 0
    size_t Length() const;      // Length in characters
    size_t Size() const;        // Size in bytes. Size() can be > than Length() !!!

    // Access as to array of characters
    size_t SizeOf(size_t charIndex) const;
    size_t OffsetOf(size_t charIndex) const;

    Char CharAt(size_t charIndex) const;
    Char operator[](size_t charIndex) const;
    Char LastChar() const;

    bool RemoveAt(size_t charIndex);
    bool InsertAt(size_t charIndex, const Char& ch);
    bool ReplaceAt(size_t charIndex, const Char& ch);

    // --- Conversion
    std::string ToAnsi() const;
    std::string ToUtf8() const;
    std::string ToNativeString() const; // ANSI for Windows and UTF8 for Posix systems
    std::wstring ToWstring() const;

    // --- Modify string
    void Clear();

    void ToLowerCase();
    void ToUpperCase();

    void Trim(const Char& space = ' ');
    void TrimLeft(const Char& space = ' ');
    void TrimRight(const Char& space = ' ');

    void Remove(const Char& ch);
    void Replace(const Char& find, const Char& replace);

    bool ReplaceString(const String& find, const String& replace);

    // Extract substring
    String Substr(size_t pos = 0, size_t count = std::string::npos) const;

    // Split & Join
    StringArray Split(const CharSet& delimiters) const;
    StringArray Split(const char* delimiters) const;
    static String Join(const StringArray& arr, const Char& delimiter);

    // Search
    bool StartsWith(const String& str) const;
    bool StartsWith(const AnsiPtr& ptr) const;
    bool StartsWith(const Utf8Ptr& ptr) const;
    bool StartsWith(const w16string& str) const;
    bool StartsWith(const w16_type* ptr) const;
    bool StartsWith(const w32string& str) const;
    bool StartsWith(const w32_type* ptr) const;
    bool StartsWith(const Char& ch) const;
    bool StartsWith(const std::string& str) const;
    bool StartsWith(const char* ptr) const;

    bool EndsWith(const String& str) const;
    bool EndsWith(const AnsiPtr& ptr) const;
    bool EndsWith(const Utf8Ptr& ptr) const;
    bool EndsWith(const w16string& str) const;
    bool EndsWith(const w16_type* ptr) const;
    bool EndsWith(const w32string& str) const;
    bool EndsWith(const w32_type* ptr) const;
    bool EndsWith(const Char& ch) const;
    bool EndsWith(const std::string& str) const;
    bool EndsWith(const char* ptr) const;

    bool Includes(const String& str, size_t pos = 0) const;
    bool Includes(const AnsiPtr& ptr, size_t pos = 0) const;
    bool Includes(const Utf8Ptr& ptr, size_t pos = 0) const;
    bool Includes(const w16string& str, size_t pos = 0) const;
    bool Includes(const w16_type* ptr, size_t pos = 0) const;
    bool Includes(const w32string& str, size_t pos = 0) const;
    bool Includes(const w32_type* ptr, size_t pos = 0) const;
    bool Includes(const Char& ch, size_t pos = 0) const;
    bool Includes(const std::string& str, size_t pos = 0) const;
    bool Includes(const char* ptr, size_t pos = 0) const;

    size_t IndexOf(const String& str, size_t Off = 0U) const;
    size_t IndexOf(const AnsiPtr& ptr, size_t Off = 0U) const;
    size_t IndexOf(const Utf8Ptr& ptr, size_t Off = 0U) const;
    size_t IndexOf(const w16string& str, size_t Off = 0U) const;
    size_t IndexOf(const w16_type* ptr, size_t Off = 0U) const;
    size_t IndexOf(const w32string& str, size_t Off = 0U) const;
    size_t IndexOf(const w32_type* ptr, size_t Off = 0U) const;
    size_t IndexOf(const Char& ch, size_t Off = 0U) const;
    size_t IndexOf(const std::string& str, size_t Off = 0U) const;
    size_t IndexOf(const char* ptr, size_t Off = 0U) const;

    size_t LastIndexOf(const String& str) const;
    size_t LastIndexOf(const AnsiPtr& ptr) const;
    size_t LastIndexOf(const Utf8Ptr& ptr) const;
    size_t LastIndexOf(const w16string& str) const;
    size_t LastIndexOf(const w16_type* ptr) const;
    size_t LastIndexOf(const w32string& str) const;
    size_t LastIndexOf(const w32_type* ptr) const;
    size_t LastIndexOf(const Char& ch) const;
    size_t LastIndexOf(const std::string& str) const;
    size_t LastIndexOf(const char* ptr) const;

    // --- operator==
    bool operator==(const String& str) const;
    bool operator==(const AnsiPtr& ptr) const;
    bool operator==(const Utf8Ptr& ptr) const;
    bool operator==(const w16string& str) const;
    bool operator==(const w16_type* ptr) const;
    bool operator==(const w32string& str) const;
    bool operator==(const w32_type* ptr) const;
    bool operator==(const Char& ch) const;
    bool operator==(const char* ptr) const;
    bool operator==(const std::string& str) const;

    bool IsEqual(const String& str) const;
    bool IsEqual(const AnsiPtr& ptr) const;
    bool IsEqual(const Utf8Ptr& ptr) const;
    bool IsEqual(const w16string& str) const;
    bool IsEqual(const w16_type* ptr) const;
    bool IsEqual(const w32string& str) const;
    bool IsEqual(const w32_type* ptr) const;
    bool IsEqual(const Char& ch) const;
    bool IsEqual(const std::string& str) const;
    bool IsEqual(const char* ptr) const;

    bool IsEqualNoCase(const String& str) const;
    bool IsEqualNoCase(const AnsiPtr& ptr) const;
    bool IsEqualNoCase(const Utf8Ptr& ptr) const;
    bool IsEqualNoCase(const w16string& str) const;
    bool IsEqualNoCase(const w16_type* ptr) const;
    bool IsEqualNoCase(const w32string& str) const;
    bool IsEqualNoCase(const w32_type* ptr) const;
    bool IsEqualNoCase(const Char& ch) const;
    bool IsEqualNoCase(const std::string& str) const;
    bool IsEqualNoCase(const char* ptr) const;

    // --- operator!=
    bool operator!=(const String& str) const;
    bool operator!=(const AnsiPtr& ptr) const;
    bool operator!=(const Utf8Ptr& ptr) const;
    bool operator!=(const w16string& str) const;
    bool operator!=(const w16_type* ptr) const;
    bool operator!=(const w32string& str) const;
    bool operator!=(const w32_type* ptr) const;
    bool operator!=(const Char& ch) const;
    bool operator!=(const char*) const;
    bool operator!=(const std::string&) const;

    // --- operator=
    String& operator=(const String& str);
    String& operator=(const AnsiPtr& ptr);
    String& operator=(const Utf8Ptr& ptr);
    String& operator=(const w16string& str);
    String& operator=(const w16_type* ptr);
    String& operator=(const w32string& str);
    String& operator=(const w32_type* ptr);
    String& operator=(const Char& ch);
    String& operator=(const char*);
    String& operator=(const std::string&);

    // --- operator+=
    String& operator+=(const String& str);
    String& operator+=(const AnsiPtr& ptr);
    String& operator+=(const Utf8Ptr& ptr);
    String& operator+=(const w16string& str);
    String& operator+=(const w16_type* ptr);
    String& operator+=(const w32string& str);
    String& operator+=(const w32_type* ptr);
    String& operator+=(const Char& ch);
    String& operator+=(const char*);
    String& operator+=(const std::string&);

    // --- operator+
    String operator+(const String& str) const;
    String operator+(const AnsiPtr& ptr) const;
    String operator+(const Utf8Ptr& ptr) const;
    String operator+(const w16string& str) const;
    String operator+(const w16_type* ptr) const;
    String operator+(const w32string& str) const;
    String operator+(const w32_type* ptr) const;
    String operator+(const Char& ch) const;
    String operator+(char ch) const;
    String operator+(const char* ptr) const;
    String operator+(const std::string&) const;

    // --- operator< (searching in map for example)
    bool operator<(const String& str) const;
    bool operator<(const AnsiPtr& str) const;
    bool operator<(const Utf8Ptr& str) const;
    bool operator<(const w16string& str) const;
    bool operator<(const w16_type* str) const;
    bool operator<(const w32string& str) const;
    bool operator<(const Char& str) const;
    bool operator<(const w32_type* str) const;
    bool operator<(char ch) const;
    bool operator<(const char* ptr) const;
    bool operator<(const std::string&) const;

    static size_t CharSize(const char* p);

    static bool Valid(const char* p);
    static bool Valid(const std::string& str);
    static const char* Verify(const char* ptr);

    // Aliases
    bool empty() const { return Empty(); }
    size_t size() const { return Size(); }
    void clear() { Clear(); }

    template<typename T> size_t find(T t) { return IndexOf(t); }
    template<typename T> size_t rfind(T t) { return LastIndexOf(t); }

  private:
    size_t PtrToPos(const char* p0) const;
    size_t PosToBitPos(const size_t& pos) const;
  };

  String operator+(const char* left, const String& str);
  String operator+(const std::string& left, const String& str);
}