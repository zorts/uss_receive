#include "textunit.h"
#include "gtest/gtest.h"
#include <string.h>

namespace {

  TEST(TextUnitValue,Create) {
  TextUnitValue* tu0 = TextUnitValue::create(10);
  ASSERT_TRUE(tu0);
  EXPECT_EQ(10, tu0->length());
  EXPECT_EQ(((const char*)tu0)+sizeof(uint16_t), (const char*)tu0->data());
  TextUnitValue::destroy(tu0);
  tu0 = 0;

  static const char* foobar = "foobar";
  tu0 = TextUnitValue::create(foobar);
  ASSERT_TRUE(tu0);
  EXPECT_EQ(strlen(foobar), tu0->length());
  EXPECT_EQ(((const char*)tu0)+sizeof(uint16_t), (const char*)tu0->data());
  EXPECT_EQ(0, strcmp(foobar, (const char*) tu0->data()));
  TextUnitValue::destroy(tu0);
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
