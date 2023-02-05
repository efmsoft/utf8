# utf8 library

Это библиотека содержит в себе набор классов по работе со строками в формате utf8, а также функции конвертации строк в форматах utf8, ANSI, utf16, utf32.

Наиболее востребованными оперециями конвертирования форматов являются конвертация из кодировки ANSI под Windows, a также из строки Unicode (под Windows это строка в кодировке utf16; для Posix систем это utf32). Данная библиотека позволяет решить эти задачи вызовом одной из реализованных в ней функции:
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

В utf8 строке символ может кодироваться одним, двумя или тремя байтами (https://en.wikipedia.org/wiki/UTF-8). Таким образом в общем случае длина строки в символах и длина строки в байтах это разные величины. Поэтому STL классы такие как std::string не подходят для ряда операций (например, для поиска и извлечения подстрок). Данная библиотека предлагает для работы с utf8 строками класс utf8::String. Этот класс во многом похож на std::string, но правильным образом реализует все операции по работе со строкой в utf8

```
utf8::String u8str(u8"Абв");
printf("Number of characters: %zu\n", u8str.length());  // 3 characters
printf("Size in bytes: %zu\n", u8str.size()); // 6 bytes

utf8::Char ch(L'Ж');
u8str.ReplaceAt(1, ch); // now u8str contains "АЖв"

```
