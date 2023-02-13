#include <gtest/gtest.h>
#include <utf8/String.h>

#include "SplitData.cpi"

using namespace utf8;

TEST(akUtf8String, Split)
{
  String lutStr(SplitData);
  StringArray splittedLutStr = lutStr.Split({' '});
}
