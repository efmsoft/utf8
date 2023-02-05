#include <gtest/gtest.h>
#include <utf8/String.h>

using namespace utf8;

extern const char* SplitData;

TEST(String, ConvertDirect)
{
  String str(SplitData);
  w32string w32 = Utf8ToUtf32(str);
}

TEST(akString, Utf8ToUtf16)
{
  auto str1 = Utf8ToUtf16("");
  EXPECT_EQ(str1, w16string());
}