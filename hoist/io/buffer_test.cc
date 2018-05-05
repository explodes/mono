#include "buffer.h"
#include "gtest/gtest.h"

TEST(Buffer, write_acceptsInput) {
  Buffer buf;

  char str[] = "hello, world";
  buf.write(str, 12);

  EXPECT_EQ(std::string("hello, world"), buf.string());
}

TEST(Buffer, write_acceptsPartialInput) {
  Buffer buf;

  char str[] = "hello, world";
  buf.write(str, 5);

  EXPECT_EQ(std::string("hello"), buf.string());
}

TEST(Buffer, string_returnsDifferentStringAfterMultipleWrites) {
  Buffer buf;

  char str1[] = "hello";
  buf.write(str1, 5);

  EXPECT_EQ(std::string("hello"), buf.string());

  char str2[] = ", world";
  buf.write(str2, 7);

  EXPECT_EQ(std::string("hello, world"), buf.string());
}

TEST(Buffer, reset_clearsContents) {
  Buffer buf;

  char str1[] = "hello";
  buf.write(str1, 5);
  buf.reset();

  EXPECT_EQ(std::string(""), buf.string());
}