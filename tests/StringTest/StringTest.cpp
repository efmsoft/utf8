#include <gtest/gtest.h>
#include <utf8/Convert.h>
#include <utf8/String.h>

#pragma warning(disable : 4566)

using namespace utf8;

const w16_type* Text1Utf16 = (w16_type*)u"тЕкст1 王明 Mötley Crüe";
const w16_type* Text1Utf16lc = (w16_type*)u"текст1 王明 mötley crüe";
const w16_type* Text1Utf16uc = (w16_type*)u"ТЕКСТ1 王明 MÖTLEY CRÜE";

const w32_type* Text1Utf32 = (w32_type*)U"тЕкст1 王明 Mötley Crüe";
const w32_type* Text1Utf32lc = (w32_type*)U"текст1 王明 mötley crüe";
const w32_type* Text1Utf32uc = (w32_type*)U"ТЕКСТ1 王明 MÖTLEY CRÜE";

#ifdef _WIN32
  #define NATIVE_WSTR Text1Utf16
  #define NATIVE_WSTRLC Text1Utf16lc
  #define NATIVE_WSTRUC Text1Utf16uc
#else //#if defined(__APPLE__)
  #define NATIVE_WSTR Text1Utf32
  #define NATIVE_WSTRLC Text1Utf32lc
  #define NATIVE_WSTRUC Text1Utf32uc
#endif

TEST(String, ConstructorAnsi)
{
  const char* text = "1234567890\xc0\xc1\xc2\xc3";
  const char* text1Ansi = text;
  
  std::string utf8 = AnsiToUtf8(text1Ansi);
  const char* text1Utf8 = utf8.c_str();

  AnsiPtr ptr(text1Ansi);
  
  String s1(ptr);
  String s2{ AnsiPtr(text1Ansi) };

  const char* c1 = s1;
  const char* c2 = s2;

  EXPECT_EQ(strcmp(c1, text1Utf8), 0);
  EXPECT_EQ(strcmp(c2, text1Utf8), 0);
}

TEST(String, ConstructorUtf8)
{
  std::string utf8 = Utf16ToUtf8(Text1Utf16);
  const char* text1Utf8 = utf8.c_str();

  Utf8Ptr ptr(text1Utf8);

  String s1(ptr);
  String s2{ Utf8Ptr(text1Utf8) };
  String s3{ text1Utf8 };

  const char* c1 = s1;
  const char* c2 = s2;
  const char* c3 = s3;

  EXPECT_EQ(strcmp(c1, text1Utf8), 0);
  EXPECT_EQ(strcmp(c2, text1Utf8), 0);
  EXPECT_EQ(strcmp(c3, text1Utf8), 0);
}

TEST(String, ConstructorUtf16)
{
  std::string utf8 = Utf16ToUtf8(Text1Utf16);
  const char* text1Utf8 = utf8.c_str();

  String s1(Text1Utf16);
  EXPECT_EQ(strcmp(s1, text1Utf8), 0);

  String s2((w16_type*)u"АБВГД", 3);
  EXPECT_EQ(s2.Str(), u8"АБВ");
}

TEST(String, ConstructorUtf32)
{
  std::string utf8 = Utf16ToUtf8(Text1Utf16);
  const char* text1Utf8 = utf8.c_str();

  String s1(Text1Utf32);
  EXPECT_EQ(strcmp(s1, text1Utf8), 0);

  String s2((w32_type*)U"АБВГД", 3);
  EXPECT_EQ(s2.Str(), u8"АБВ");
}

TEST(String, Constructor_Char)
{
  String s1('1', 6);
  EXPECT_EQ(s1.Str(), "111111");

  Char ch(L'Ж');
  String s2(ch, 2);
  EXPECT_EQ(s2.Str(), u8"ЖЖ");
}

TEST(String, LengthNeSize)
{
  String s1(Text1Utf16);

  size_t len = s1.Length();
  size_t cb = s1.Size();
  EXPECT_LT(len, cb);
}

TEST(String, Length)
{
  String s1((w16_type*)u"王明 Mötley Crüe");
  EXPECT_EQ(s1.Length(), 14);
}

TEST(String, SizeOf)
{
  String s1(Text1Utf16);
  EXPECT_EQ(s1.SizeOf(4), 2); // 'т'
  EXPECT_EQ(s1.SizeOf(5), 1); // '1'
}

TEST(String, OffsetOf)
{
  String s1(Text1Utf16);
  EXPECT_EQ(s1.OffsetOf(0), 0);
  EXPECT_EQ(s1.OffsetOf(1), 2);
}

TEST(String, Item)
{
  String s1(Text1Utf32);

  EXPECT_EQ(s1[0].size(), 2);
  EXPECT_EQ(s1[0], Char(L'т'));

  EXPECT_EQ(s1[5].size(), 1);
  EXPECT_EQ(s1[5], Char('1'));
}

TEST(String, RemoveAt)
{
  String s1(L"_АБ_");

  EXPECT_EQ(s1.RemoveAt(1), true);
  EXPECT_EQ(s1, L"_Б_");

  EXPECT_EQ(s1.RemoveAt(1), true);
  EXPECT_EQ(s1, L"__");

  EXPECT_EQ(s1.RemoveAt(0), true);
  EXPECT_EQ(s1, L"_");

  EXPECT_EQ(s1.RemoveAt(0), true);
  EXPECT_EQ(s1, L"");
}

TEST(String, InsertAt)
{
  String s1(L"_Б_");

  Char chA(L'А');
  EXPECT_EQ(s1.InsertAt(1, chA), true);
  EXPECT_EQ(s1, L"_АБ_");

  Char chB(U'В');
  EXPECT_EQ(s1.InsertAt(3, chB), true);
  EXPECT_EQ(s1, (w32_type*)U"_АБВ_");

  Char ch_('_');
  EXPECT_EQ(s1.InsertAt(std::string::npos, ch_), true);
  EXPECT_EQ(s1, (w16_type*)u"_АБВ__");
}

TEST(String, ReplaceAt)
{
  String s1(L"_АБВ_");

  EXPECT_EQ(s1.ReplaceAt(0, '-'), true);
  EXPECT_EQ(s1, L"-АБВ_");

  EXPECT_EQ(s1.ReplaceAt(4, '-'), true);
  EXPECT_EQ(s1, L"-АБВ-");

  EXPECT_EQ(s1.ReplaceAt(2, L'Х'), true);
  EXPECT_EQ(s1, L"-АХВ-");
}

TEST(String, ConversionAnsi)
{
  std::string ansi = Utf16ToAnsi((w16_type*)u"abcdef");
  const char* text1Ansi = ansi.c_str();

  String s1{ AnsiPtr(text1Ansi) };
  EXPECT_EQ(strcmp(s1.ToAnsi().c_str(), text1Ansi), 0);
}

TEST(String, ConversionUtf8)
{
  std::string utf8 = Utf16ToUtf8(Text1Utf16);
  const char* text1Utf8 = utf8.c_str();

  String s1(utf8);
  EXPECT_EQ(strcmp(s1.ToUtf8().c_str(), text1Utf8), 0);
}

TEST(String, ConversionWstring)
{
  std::string utf8 = Utf16ToUtf8(Text1Utf16);

  String s1(utf8);
  EXPECT_EQ(wcscmp(s1.ToWstring().c_str(), NATIVE_WSTR), 0);
}

TEST(String, ToLowerCase)
{
  String s1(u8"РусСкиЙ");
  s1.ToLowerCase();
  EXPECT_EQ(s1, u8"русский");

  String s2(u8"äöüßÄÖÜ");
  s2.ToLowerCase();
  EXPECT_EQ(s2, u8"äöüßäöü");

  String s3(u8"TestString");
  s3.ToLowerCase();
  EXPECT_EQ(s3, u8"teststring");
}

TEST(String, ToUpperCase)
{
  String s1(u8"РусСкиЙ");
  s1.ToUpperCase();
  EXPECT_EQ(s1, u8"РУССКИЙ");

  String s2(u8"äöüßÄÖÜ");
  s2.ToUpperCase();
  EXPECT_EQ(s2, u8"ÄÖÜßÄÖÜ");

  String s3(u8"TestString");
  s3.ToUpperCase();
  EXPECT_EQ(s3, u8"TESTSTRING");
}

TEST(String, OperatorConstruct)
{
  String s2((w16_type *)u"2");

  String s;
  s += String((w32_type*)U"1 + ") + s2 + AnsiPtr(" = ") + '3';

  EXPECT_EQ(wcscmp(s.ToWstring().c_str(), L"1 + 2 = 3"), 0);
}

TEST(String, IsEqualNoCase)
{
  String s1(Text1Utf32);
  String s2(Text1Utf16uc);
  String s3((w16_type*)u"any");

  EXPECT_EQ(s1.IsEqualNoCase(s2), true);
  EXPECT_EQ(s1.IsEqualNoCase(s3), false);
}

TEST(String, Substr)
{
  String str(L"abc");
  str += String(L"def") + L"0123456789";

  String all = str.Substr();
  String digits = str.Substr(6);
  String dig012 = str.Substr(6, 3);

  EXPECT_EQ(all, str);
  EXPECT_EQ(digits, L"0123456789");
  EXPECT_EQ(dig012, L"012");
}

TEST(String, StartsWith)
{
  String str(L"abc");

  EXPECT_EQ(str.StartsWith('a'), true);
  EXPECT_EQ(str.StartsWith((w16_type*)u"ab"), true);
  EXPECT_EQ(str.StartsWith((w32_type*)U"abc"), true);

  EXPECT_EQ(str.StartsWith((w16_type*)u"b"), false);
  EXPECT_EQ(str.StartsWith('x'), false);
}

TEST(String, EndsWith)
{
  String str(L"abc");

  EXPECT_EQ(str.EndsWith('c'), true);
  EXPECT_EQ(str.EndsWith((w16_type*)u"bc"), true);
  EXPECT_EQ(str.EndsWith((w32_type*)U"abc"), true);

  EXPECT_EQ(str.EndsWith((w16_type*)u"b"), false);
  EXPECT_EQ(str.EndsWith('x'), false);
}

TEST(String, Includes)
{
  String str(L"abc");

  EXPECT_EQ(str.Includes('c'), true);
  EXPECT_EQ(str.Includes((w16_type*)u"bc"), true);
  EXPECT_EQ(str.Includes((w32_type*)U"abc"), true);

  EXPECT_EQ(str.Includes('c', 1), true);
  EXPECT_EQ(str.Includes('c', 3), false);
  EXPECT_EQ(str.Includes((w16_type*)u"bc", 1), true);
  EXPECT_EQ(str.Includes((w16_type*)u"bc", 2), false);
  EXPECT_EQ(str.Includes((w32_type*)U"abc", 1), false);
}

TEST(String, IndexOf)
{
  String str(L"abcabc");

  EXPECT_EQ(str.IndexOf('c'), 2);
  EXPECT_EQ(str.IndexOf((w16_type*)u"bc"), 1);
  EXPECT_EQ(str.IndexOf((w32_type*)U"abc"), 0);
  EXPECT_EQ(str.IndexOf('x'), std::string::npos);
}

TEST(String, LastIndexOf)
{
  String str(L"abcabc");

  EXPECT_EQ(str.LastIndexOf('c'), 5);
  EXPECT_EQ(str.LastIndexOf((w16_type*)u"bc"), 4);
  EXPECT_EQ(str.LastIndexOf((w32_type*)U"abc"), 3);
  EXPECT_EQ(str.LastIndexOf('x'), std::string::npos);
}

TEST(String, CompareEq)
{
  String str((w16_type*)u"абвгд");
  String str2('2', 3);

  EXPECT_EQ(str == (w16_type*)u"абвгд", true);
  EXPECT_EQ(str == (w32_type*)U"абвгд", true);
  EXPECT_EQ(str == u8"абвгд", true);
  EXPECT_EQ(str == '1', false);
  EXPECT_EQ(str == str2, false);

  EXPECT_EQ(str2 == String(L"222"), true);
  EXPECT_EQ(str2 == u8"абвгд", false);
}

TEST(String, CompareNe)
{
  String str((w16_type*)u"абвгд");
  String str2('2', 3);

  EXPECT_EQ(str != (w16_type*)u"абвгд", false);
  EXPECT_EQ(str != (w32_type*)U"абвгд", false);
  EXPECT_EQ(str != u8"абвгд", false);
  EXPECT_EQ(str != '1', true);
  EXPECT_EQ(str != str2, true);

  EXPECT_EQ(str2 != String(L"222"), false);
  EXPECT_EQ(str2 != Utf8Ptr(u8"абвгд"), true);
}

TEST(String, SplitJoin)
{
  String str1((w16_type*)u"щабщвгщд");
  StringArray a1 = str1.Split(u8"щ");
  StringArray a2{ L"", L"аб", L"вг", L"д" };
  EXPECT_EQ(a1, a2);

  String str2 = String::Join(a1, '_');
  EXPECT_EQ(str2, L"_аб_вг_д");

  String str3("first,second,third");
  StringArray a3 = str3.Split(",");
  StringArray a4{ L"first", L"second", L"third" };
  EXPECT_EQ(a3, a4);

  String str4 = String::Join(a3, ':');
  EXPECT_EQ(str4, L"first:second:third");
}

TEST(String, TrimLeft)
{
  String str1;
  str1.TrimLeft();
  EXPECT_EQ(str1.Empty(), true);

  String str2(L"  test  ");
  str2.TrimLeft();
  EXPECT_EQ(str2, L"test  ");

  String str3(L"test  ");
  str3.TrimLeft();
  EXPECT_EQ(str3, L"test  ");

  String str4(L"  ");
  str4.TrimLeft();
  EXPECT_EQ(str4.Empty(), true);

  String str5(L"test");
  str5.TrimLeft('t');
  EXPECT_EQ(str5, L"est");
}

TEST(String, TrimRight)
{
  String str1;
  str1.TrimRight();
  EXPECT_EQ(str1.Empty(), true);

  String str2(L"  test  ");
  str2.TrimRight();
  EXPECT_EQ(str2, L"  test");

  String str3(L"  test");
  str3.TrimRight();
  EXPECT_EQ(str3, L"  test");

  String str4(L"  ");
  str4.TrimRight();
  EXPECT_EQ(str4.Empty(), true);

  String str5(L"test");
  str5.TrimRight('t');
  EXPECT_EQ(str5, L"tes");
}

TEST(String, Trim)
{
  String str1;
  str1.Trim();
  EXPECT_EQ(str1.Empty(), true);

  String str2(L"  test  ");
  str2.Trim();
  EXPECT_EQ(str2, L"test");

  String str3(L"test");
  str3.Trim('t');
  EXPECT_EQ(str3, L"es");
}

TEST(String, Char)
{
  String str1;
  str1.Remove('t');
  EXPECT_EQ(str1.Empty(), true);

  String str2("  test  ");
  str2.Remove('t');
  EXPECT_EQ(str2, u8"  es  ");

  String str3("  ttttt  ");
  str3.Trim();
  str3.Remove('t');
  EXPECT_EQ(str3.Empty(), true);

  String str4(u8"ГроГ");
  str4.Remove(L'Г');
  EXPECT_EQ(str4, u8"ро");

  {
	  String str(u8"Test\\5|4:7<<z69/ae\"z|of*Lkd?é!!?*L:k//><> \\\" ");
	  str.Remove(u8'<');
	  str.Remove(u8'>');
	  str.Remove(u8':');
	  str.Remove(u8'"');
	  str.Remove(u8'/');
	  str.Remove(u8'\\');
	  str.Remove(u8'|');
	  str.Remove(u8'?');
	  str.Remove(u8'*');

	  EXPECT_EQ(str, u8"Test547z69aezofLkdé!!Lk  ");
  }

  {
	  String str(u8"Test\\5|4:7<<z69/ae\"z|of*Lkd?é!!?*L:k//><> \\\" ");
	  str.Remove('<');
	  str.Remove('>');
	  str.Remove(':');
	  str.Remove('"');
	  str.Remove('/');
	  str.Remove('\\');
	  str.Remove('|');
	  str.Remove('?');
	  str.Remove('*');

	  EXPECT_EQ(str, u8"Test547z69aezofLkdé!!Lk  ");
  }

  {
	  String str(u8" rsfs \1 \2 \xa qed");
	  unsigned int i = 1;
	  str.Remove((char)i);
	  i = 10;
	  str.Remove((char)i);

	  EXPECT_EQ(str, u8" rsfs  \2  qed");
  }
}