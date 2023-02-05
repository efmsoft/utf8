#include <gtest/gtest.h>
#include <utf8/String.h>

using namespace utf8;

TEST(String, xstrlen)
{
  EXPECT_EQ(w16_strlen(u"abcd"), 4);
  EXPECT_EQ(w16_strlen(u""), 0);

  EXPECT_EQ(w32_strlen(U"abcde"), 5);
  EXPECT_EQ(w32_strlen(U""), 0);
}

TEST(String, xstrcpy)
{
  char16_t b16[16];
  w16_strcpy(b16, u"abcd-СЛОВО");

  String str16((w16_type*)b16);
  EXPECT_EQ(str16.Length(), 10);

  char32_t b32[16]{};
  w32_strcpy((w32_type*)b32, (const w32_type*)U"тест-王明");

  String str32((w32_type*)b32);
  EXPECT_EQ(str32.Length(), 7);
}

TEST(String, xstrncpy)
{
  char16_t b16[7] = { 1, 2, 3, 4, 5, 6, 0xAA};
  w16_strncpy(b16, u"abcd", 6);
  EXPECT_EQ(b16[0], 'a');
  EXPECT_EQ(b16[1], 'b');
  EXPECT_EQ(b16[2], 'c');
  EXPECT_EQ(b16[3], 'd');
  EXPECT_EQ(b16[4], 0);
  EXPECT_EQ(b16[5], 0);
  EXPECT_EQ(b16[6], 0xAA);

  char32_t b32[7] = { 1, 2, 3, 4, 5, 6, 0xAA };
  w32_strncpy((w32_type*)b32, (const w32_type*)U"abcdef", 6);
  EXPECT_EQ(b32[0], 'a');
  EXPECT_EQ(b32[1], 'b');
  EXPECT_EQ(b32[2], 'c');
  EXPECT_EQ(b32[3], 'd');
  EXPECT_EQ(b32[4], 'e');
  EXPECT_EQ(b32[5], 'f');
  EXPECT_EQ(b32[6], 0xAA);
}

TEST(String, xstrncpyz)
{
  char16_t b16[7] = { 1, 2, 3, 4, 5, 6, 0xAA };
  w16_strncpyz(b16, u"abcd", 6);
  EXPECT_EQ(b16[0], 'a');
  EXPECT_EQ(b16[1], 'b');
  EXPECT_EQ(b16[2], 'c');
  EXPECT_EQ(b16[3], 'd');
  EXPECT_EQ(b16[4], 0);
  EXPECT_EQ(b16[5], 0);
  EXPECT_EQ(b16[6], 0xAA);

  char32_t b32[7] = { 1, 2, 3, 4, 5, 6, 0xAA };
  w32_strncpyz((w32_type*)b32, (const w32_type*)U"abcdef", 6);
  EXPECT_EQ(b32[0], 'a');
  EXPECT_EQ(b32[1], 'b');
  EXPECT_EQ(b32[2], 'c');
  EXPECT_EQ(b32[3], 'd');
  EXPECT_EQ(b32[4], 'e');
  EXPECT_EQ(b32[5], 0);
  EXPECT_EQ(b32[6], 0xAA);
}