#include "hoist/logging.h"

#include <iostream>

#include "gtest/gtest.h"

namespace {

using std::ostream;

class Loggable {
 public:
  Loggable() : logged_(false) {}

  bool logged() const { return logged_; }

  friend ostream& operator<<(ostream&, const Loggable&);

 private:
  mutable bool logged_;
};

ostream& operator<<(ostream& out, const Loggable& b) {
  b.logged_ = true;
  return out << "~hello~";
}

TEST(Logging, ELOG) {
  Loggable loggable;

  ELOG(loggable << loggable);

  EXPECT_TRUE(loggable.logged());
}

TEST(Logging, ELOG_IF_true) {
  Loggable loggable;

  ELOG_IF(true, loggable << loggable);

  EXPECT_TRUE(loggable.logged());
}

TEST(Logging, ELOG_IF_false) {
  Loggable loggable;

  ELOG_IF(false, loggable << loggable);

  EXPECT_FALSE(loggable.logged());
}

TEST(Logging, WLOG) {
  Loggable loggable;

  WLOG(loggable << loggable);

  EXPECT_TRUE(loggable.logged());
}

TEST(Logging, WLOG_IF_true) {
  Loggable loggable;

  WLOG_IF(true, loggable << loggable);

  EXPECT_TRUE(loggable.logged());
}

TEST(Logging, WLOG_IF_false) {
  Loggable loggable;

  WLOG_IF(false, loggable << loggable);

  EXPECT_FALSE(loggable.logged());
}

TEST(Logging, ILOG) {
  Loggable loggable;

  ILOG(loggable << loggable);

  EXPECT_TRUE(loggable.logged());
}

TEST(Logging, ILOG_IF_true) {
  Loggable loggable;

  ILOG_IF(true, loggable << loggable);

  EXPECT_TRUE(loggable.logged());
}

TEST(Logging, ILOG_IF_false) {
  Loggable loggable;

  ILOG_IF(false, loggable << loggable);

  EXPECT_FALSE(loggable.logged());
}

TEST(Logging, DLOG) {
  Loggable loggable;

  DLOG(loggable << loggable);

  EXPECT_TRUE(loggable.logged());
}

TEST(Logging, DLOG_IF_true) {
  Loggable loggable;

  DLOG_IF(true, loggable << loggable);

  EXPECT_TRUE(loggable.logged());
}

TEST(Logging, DLOG_IF_false) {
  Loggable loggable;

  DLOG_IF(false, loggable << loggable);

  EXPECT_FALSE(loggable.logged());
}

TEST(Logging, PRINT) {
  Loggable loggable;

  PRINT(loggable);

  EXPECT_TRUE(loggable.logged());
}

}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}