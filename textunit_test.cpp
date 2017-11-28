// -*-C++-*-
#include "textunit.h"
#include "textunits.h"
#include "gtest/gtest.h"
#include <string.h>

namespace {

  TEST(TextUnitValue,Create) {
    TextUnitValue* tuv0 = TextUnitValue::create(10);
    ASSERT_TRUE(tuv0);
    EXPECT_EQ(10, tuv0->length());
    EXPECT_EQ(((const char*)tuv0)+sizeof(uint16_t), (const char*)tuv0->data());
    TextUnitValue::destroy(tuv0);
    tuv0 = 0;

    static const char* foobar = "foobar";
    tuv0 = TextUnitValue::create(foobar);
    ASSERT_TRUE(tuv0);
    EXPECT_EQ(strlen(foobar), tuv0->length());
    EXPECT_EQ(((const char*)tuv0)+sizeof(uint16_t), (const char*)tuv0->data());
    EXPECT_EQ(0, strcmp(foobar, (const char*) tuv0->data()));
    TextUnitValue::destroy(tuv0);
  }

  TEST(TextUnit,Create) {
    TextUnit* tu0 = TextUnit::create(42, {"foo", "bar", "bas"});
    ASSERT_TRUE(tu0);
    EXPECT_EQ(42, tu0->key());
    EXPECT_EQ(3, tu0->number());
    TextUnit::destroy(tu0);
    tu0 = 0;

  }

  TEST(TextUnitDictionary,Create) {
    TextUnitDictionary* dict = new TextUnitDictionary;
    ASSERT_TRUE(dict);
    ASSERT_TRUE(dict->size());
    delete dict;
  }


}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
