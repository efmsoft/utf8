# utf8 library

This library contains a set of classes for working with strings in utf8 format, as well as functions for converting strings in utf8, ANSI, utf16, utf32 formats.

The most commonly used format conversion operations are converting from ANSI encoding (on Windows), as well as from a Unicode string (on Windows it is a utf-16 encoded string; on Posix systems it is utf-32). This library allows to solve these problems by calling one of the conversion functions:
```
const wchar_t* unicode_str = L"тЕкст1 王明 Mötley Crüe";

std::string utf8_str = utf8::WstringToUtf8(unicode_str);
std::wstring unicode_str2 = utf8::Utf8ToWstring(utf8_str.c_str());
assert(wcscmp(unicode_str, unicode_str2.c_str()) == 0);

#ifdef _WIN32
 WIN32_FIND_DATAA fd;
 HANDLE h = FindFirstFileA(mask, &fd);
 if (h != INVALID_HANDLE_VALUE)
 {
   std::string u8name = utf8::AnsiToUtf8(fd.cFileName);
   FindClose(h);
 }
#endif
```

In a utf-8 string, a character can be encoded with one, two, or three bytes (https://en.wikipedia.org/wiki/UTF-8). Thus, in general, the length of a string in characters and the length of a string in bytes are different values. Therefore, STL classes such as std::string are not suitable for a number of operations (for example, searching and extracting substrings). This library offers the utf8::String class for working with utf8 strings. This class is similar to std::string in many ways, but correctly implements all operations on working with a string in utf8

```
utf8::String u8str(u8"Абв");
printf("Number of characters: %zu\n", u8str.length());  // 3 characters
printf("Size in bytes: %zu\n", u8str.size()); // 6 bytes

utf8::Char ch(L'Ж');
u8str.ReplaceAt(1, ch); // now u8str contains "АЖв"

```
