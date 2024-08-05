# utf8 library

This library contains a set of classes for working with strings in **UTF-8** format, as well as functions for converting strings in **utf8**, **ANSI**, **utf16**, **utf32** formats.

## Conversion from/to UTF8
The most commonly used format conversion operations are converting from **ANSI** encoding (on Windows), as well as from a **Unicode** string (on Windows it is a **utf-16** encoded string; on Posix systems it is **utf-32**). This library allows to solve these problems by calling one of the conversion functions:
```cpp
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
## utf8::String class
In a **utf-8** string, a character can be encoded with one, two, or three bytes (https://en.wikipedia.org/wiki/UTF-8). Thus, in general, the length of a string in characters and the length of a string in bytes are different values. Therefore, STL classes such as std::string are not suitable for a number of operations (for example, searching and extracting substrings). This library offers the **utf8::String** class for working with utf8 strings. This class is similar to **std::string** in many ways, but correctly implements all operations on working with a string in utf8

```cpp
utf8::String u8str(u8"Абв");
printf("Number of characters: %zu\n", u8str.length());  // 3 characters
printf("Size in bytes: %zu\n", u8str.size()); // 6 bytes

utf8::Char ch(L'Ж');
u8str.ReplaceAt(1, ch); // now u8str contains "АЖв"

```
One of the problems when working with characters in a national encoding is the conversion of the character case. The utf8 library implements these methods for both **Windows** and **Linux**. Thus, the **ToLowerCase** and **ToUpperCase** methods of the utf8::String class are able to correctly perform such a conversion and do not require changes in С **locale**.

## utf8::Char class
**Utf-8** characters can occupy more than one byte. Thus, the built-in C **char** type is not fully suitable for storing utf8 characters. Since some of the **utf8::String** class methods accept or return a single character, the library defines a special type for storing a single character - **utf8::Char**.

## Utf8Ptr and AnsiPtr Classes
As is known, a character is encoded by 8 bits in several encodings at once. This is the **Utf8** encoding, and the **ANSI** and **latin** encoding. In the C code, all this corresponds to **"const char*"**. In order to be able to distinguish these types when passing a pointer to a string to the methods of the **utf8::String** class, the helper classes **Utf8Ptr** and **AnsiPtr** are introduced.

```cpp
utf8::String u8str(AnsiPtr("тест в кодировке ANSI"));
```
Understanding the type of the string received as input, the **utf8::String** class performs the appropriate conversion

