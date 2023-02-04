#pragma once

template<typename CharType>
size_t xstrlen(const CharType* p)
{
  size_t n = 0;
  while (*p++)
    n++;
  return n;
}

template<typename CharType>
CharType* xstrcpy(CharType* dest, const CharType* src)
{
  CharType* p = dest;

  while (*src)
    *dest++ = *src++;
  *dest = '\0';

  return p;
}

template<typename CharType>
CharType* xstrncpy(CharType* dest, const CharType* src, size_t num)
{
  CharType* p = dest;

  for (; *src && num; --num)
    *dest++ = *src++;

  while (num--)
    *dest++ = '\0';

  return p;
}

template<typename CharType>
CharType* xstrncpyz(CharType* dest, const CharType* src, size_t num)
{
  size_t size = num;
  CharType* p = dest;

  for (; *src && num; --num)
    *dest++ = *src++;

  while (num--)
    *dest++ = '\0';

  if (size)
    p[size - 1] = '\0';

  return p;
}

