#include "webs/kv/storage.h"

#include "gtest/gtest.h"

class StorageTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    storage = new Storage(":memory");
    storage->init();
  }

  virtual void TearDown() { delete storage; }

  Storage* storage;
};

TEST_F(StorageTest, Get_DoesNotExist_NotOk) {
  auto result = storage->get("test");

  EXPECT_FALSE(result.ok());
}

TEST_F(StorageTest, Get_Exists_Ok) {
  ASSERT_TRUE(storage->put("test", "ok").ok());

  auto result = storage->get("test");

  EXPECT_TRUE(result.ok());
}

TEST_F(StorageTest, Get_Exists_Value) {
  ASSERT_TRUE(storage->put("test", "ok").ok());

  auto result = storage->get("test");

  ASSERT_TRUE(result.ok());
  EXPECT_EQ(result.ValueOrDie(), "ok");
}

TEST_F(StorageTest, Set_DoesNotExist_Ok) {
  auto result = storage->put("test", "ok");

  EXPECT_TRUE(result.ok());
}

TEST_F(StorageTest, Set_Exists_Ok) {
  ASSERT_TRUE(storage->put("test", "ok1").ok());

  auto result = storage->put("test", "ok2");

  EXPECT_TRUE(result.ok());
}

TEST_F(StorageTest, Set_Exists_Value) {
  ASSERT_TRUE(storage->put("test", "ok1").ok());
  ASSERT_TRUE(storage->put("test", "ok2").ok());

  auto result = storage->get("test");

  ASSERT_TRUE(result.ok());
  EXPECT_EQ(result.ValueOrDie(), "ok2");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}