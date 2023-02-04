#pragma once

#include <set>
#include <string>
#include <vector>

namespace utf8
{
  struct Char : public std::vector<char>
  {
    Char();
    Char(const Char& ch);       // UTF-8
    Char(char ch);              // ANSI / LATIN
    Char(wchar_t ch);           // Windows: UTF-16, Posix: UTF-32
    Char(char16_t ch);          // UTF-16
    Char(char32_t ch);          // UTF-32

    std::string Str() const;
    bool IsDigit() const;

    bool operator==(const Char& ch) const;
    bool operator==(char ch) const;
    bool operator==(wchar_t ch) const;
    bool operator==(char16_t ch) const;
    bool operator==(char32_t ch) const;

    bool operator!=(const Char& ch) const;
    bool operator!=(char ch) const;
    bool operator!=(wchar_t ch) const;
    bool operator!=(char16_t ch) const;
    bool operator!=(char32_t ch) const;

  protected:
    void AppendString(const std::string& str);
  };

  typedef std::set<Char> CharSet;
}