#include "net/ezdb/page.h"

#include "gtest/gtest.h"
#include "hoist/status.h"
#include "hoist/statusor.h"

namespace ezdb {
namespace {

using ::Hoist::Status;
using ::Hoist::StatusOr;

class PageTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  // void TearDown() override {}
  Page page_;
};

TEST_F(PageTest, IsEmptyInitially) { EXPECT_EQ(page_.NumEntries(), 0); }

TEST_F(PageTest, SetAndGet) {
  Page::Key key{'k', 'e', 'y'};
  Page::Data data{'v', 'a', 'l', 'u', 'e'};

  Status set_status = page_.Set(key, data);
  ASSERT_TRUE(set_status.ok());

  EXPECT_EQ(page_.NumEntries(), 1);

  StatusOr<Page::Data> get_status = page_.Get(key);
  ASSERT_TRUE(get_status.ok());

  Page::Data value = get_status.ValueOrDie();
  EXPECT_EQ(value, data);
}

TEST_F(PageTest, NumEntries) {
  Page::Key key_a{'k', 'e', 'y', 'a'};
  Page::Key key_b{'k', 'e', 'y', 'b'};
  Page::Key key_z{'k', 'e', 'y', 'z'};
  Page::Data data{'v', 'a', 'l', 'u', 'e'};

  EXPECT_EQ(page_.NumEntries(), 0);

  ASSERT_TRUE(page_.Set(key_a, data).ok());
  EXPECT_EQ(page_.NumEntries(), 1);

  ASSERT_TRUE(page_.Set(key_b, data).ok());
  EXPECT_EQ(page_.NumEntries(), 2);

  // Overwrite the last key_b.
  ASSERT_TRUE(page_.Set(key_b, data).ok());
  EXPECT_EQ(page_.NumEntries(), 2);

  ASSERT_TRUE(page_.Set(key_z, data).ok());
  EXPECT_EQ(page_.NumEntries(), 3);
}

TEST_F(PageTest, FirstAndLastKey) {
  Page::Key key_a{'k', 'e', 'y', 'a'};
  Page::Key key_z{'k', 'e', 'y', 'z'};
  Page::Data data{'v', 'a', 'l', 'u', 'e'};
  ASSERT_TRUE(page_.Set(key_a, data).ok());
  ASSERT_TRUE(page_.Set(key_z, data).ok());

  EXPECT_EQ(page_.FirstKey(), key_a);
  EXPECT_EQ(page_.LastKey(), key_z);
}

TEST_F(PageTest, FirstAndLastKey_Ordered) {
  Page::Key key_a{'k', 'e', 'y', 'a'};
  Page::Key key_z{'k', 'e', 'y', 'z'};
  Page::Data data{'v', 'a', 'l', 'u', 'e'};
  // Insert key_z first, then key_a.
  ASSERT_TRUE(page_.Set(key_z, data).ok());
  ASSERT_TRUE(page_.Set(key_a, data).ok());

  // key_a should still be the first key.
  EXPECT_EQ(page_.FirstKey(), key_a);
  EXPECT_EQ(page_.LastKey(), key_z);
}

TEST_F(PageTest, NoFirstAndLastKey) {
  ASSERT_EQ(page_.FirstKey(), Page::kNoKey);
  ASSERT_EQ(page_.LastKey(), Page::kNoKey);
}

}  // namespace
}  // namespace ezdb

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}