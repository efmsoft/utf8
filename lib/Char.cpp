#include <utf8/Char.h>
#include <utf8/Convert.h>

using namespace utf8;

Char::Char()
{
}

Char::Char(const Char& ch)
 : std::vector<char>(ch)
{
}

Char::Char(char ch)
{
  char str[2]{};
  str[0] = ch;
  str[1] = '\0';

  AppendString(utf8::AnsiToUtf8(str));
}

Char::Char(wchar_t ch)
{
  wchar_t str[2]{};
  str[0] = ch;
  str[1] = '\0';

#ifdef _WIN32
  std::string s = utf8::Utf16ToUtf8((const w16_type*)str);
#else
  std::string s = utf8::Utf32ToUtf8((const w32_type*)str);
#endif

  AppendString(s);
}

Char::Char(char16_t ch)
{
  char16_t str[2]{};
  str[0] = ch;
  str[1] = '\0';

  AppendString(utf8::Utf16ToUtf8((const w16_type*)str));
}

Char::Char(char32_t ch)
{
  char32_t str[2]{};
  str[0] = ch;
  str[1] = '\0';

  AppendString(utf8::Utf32ToUtf8((const w32_type*)str));
}

std::string Char::Str() const
{
  const std::vector<char>& arr = *this;
  return std::string(&arr[0], size());
}

bool Char::IsDigit() const 
{
  const std::vector<char>& arr = *this;
  if (arr.size() == 1)
  {
    return arr[0] >= '0' && arr[0] <= '9';
  }

  return false;
}

void Char::AppendString(const std::string& str)
{
  for (auto ch : str)
    push_back(ch);
}

bool Char::operator==(const Char& ch) const
{
  const std::vector<char>& arr = *this;
  return arr == ch;
}

bool Char::operator==(char ch) const
{
  return operator==(Char(ch));
}

bool Char::operator==(wchar_t ch) const
{
  return operator==(Char(ch));
}

bool Char::operator==(char16_t ch) const
{
  return operator==(Char(ch));
}

bool Char::operator==(char32_t ch) const
{
  return operator==(Char(ch));
}

bool Char::operator!=(const Char& ch) const
{
  return !operator==(ch);
}

bool Char::operator!=(char ch) const
{
  return !operator==(ch);
}

bool Char::operator!=(wchar_t ch) const
{
  return !operator==(ch);
}

bool Char::operator!=(char16_t ch) const
{
  return !operator==(ch);
}

bool Char::operator!=(char32_t ch) const
{
  return !operator==(ch);
}