#include "db/bolt/statusor.h"

#include "db/bolt/status.h"
#include "gtest/gtest.h"

namespace bolt {
namespace {

class Base1 {
 public:
  virtual ~Base1() {}
  int pad;
};

class Base2 {
 public:
  virtual ~Base2() {}
  int yetotherpad;
};

class Derived : public Base1, public Base2 {
 public:
  virtual ~Derived() {}
  int evenmorepad;
};

class CopyNoAssign {
 public:
  explicit CopyNoAssign(int value) : foo(value) {}
  CopyNoAssign(const CopyNoAssign& other) : foo(other.foo) {}
  int foo;

 private:
  const CopyNoAssign& operator=(const CopyNoAssign&);
};

TEST(StatusOr, TestDefaultCtor) {
  StatusOr<int> thing;
  EXPECT_FALSE(thing.ok());
  EXPECT_EQ(Status::UNKNOWN, thing.status());
}

TEST(StatusOr, TestStatusCtor) {
  StatusOr<int> thing(Status::CANCELLED);
  EXPECT_FALSE(thing.ok());
  EXPECT_EQ(Status::CANCELLED, thing.status());
}

TEST(StatusOr, TestValueCtor) {
  const int kI = 4;
  StatusOr<int> thing(kI);
  EXPECT_TRUE(thing.ok());
  EXPECT_EQ(kI, thing.ValueOrDie());
}

TEST(StatusOr, TestCopyCtorStatusOk) {
  const int kI = 4;
  StatusOr<int> original(kI);
  StatusOr<int> copy(original);
  EXPECT_EQ(original.status(), copy.status());
  EXPECT_EQ(original.ValueOrDie(), copy.ValueOrDie());
}

TEST(StatusOr, TestCopyCtorStatusNotOk) {
  StatusOr<int> original(Status::CANCELLED);
  StatusOr<int> copy(original);
  EXPECT_EQ(original.status(), copy.status());
}

TEST(StatusOr, TestCopyCtorStatusOKConverting) {
  const int kI = 4;
  StatusOr<int> original(kI);
  StatusOr<double> copy(original);
  EXPECT_EQ(original.status(), copy.status());
  EXPECT_EQ(original.ValueOrDie(), copy.ValueOrDie());
}

TEST(StatusOr, TestCopyCtorStatusNotOkConverting) {
  StatusOr<int> original(Status::CANCELLED);
  StatusOr<double> copy(original);
  EXPECT_EQ(original.status(), copy.status());
}

TEST(StatusOr, TestAssignmentStatusOk) {
  const int kI = 4;
  StatusOr<int> source(kI);
  StatusOr<int> target;
  target = source;
  EXPECT_EQ(source.status(), target.status());
  EXPECT_EQ(source.ValueOrDie(), target.ValueOrDie());
}

TEST(StatusOr, TestAssignmentStatusNotOk) {
  StatusOr<int> source(Status::CANCELLED);
  StatusOr<int> target;
  target = source;
  EXPECT_EQ(source.status(), target.status());
}

TEST(StatusOr, TestAssignmentStatusOKConverting) {
  const int kI = 4;
  StatusOr<int> source(kI);
  StatusOr<double> target;
  target = source;
  EXPECT_EQ(source.status(), target.status());
  EXPECT_DOUBLE_EQ(source.ValueOrDie(), target.ValueOrDie());
}

TEST(StatusOr, TestAssignmentStatusNotOkConverting) {
  StatusOr<int> source(Status::CANCELLED);
  StatusOr<double> target;
  target = source;
  EXPECT_EQ(source.status(), target.status());
}

TEST(StatusOr, TestStatus) {
  StatusOr<int> good(4);
  EXPECT_TRUE(good.ok());
  StatusOr<int> bad(Status::CANCELLED);
  EXPECT_FALSE(bad.ok());
  EXPECT_EQ(Status::CANCELLED, bad.status());
}

TEST(StatusOr, TestValue) {
  const int kI = 4;
  StatusOr<int> thing(kI);
  EXPECT_EQ(kI, thing.ValueOrDie());
}

TEST(StatusOr, TestValueConst) {
  const int kI = 4;
  const StatusOr<int> thing(kI);
  EXPECT_EQ(kI, thing.ValueOrDie());
}

TEST(StatusOr, TestPointerDefaultCtor) {
  StatusOr<int*> thing;
  EXPECT_FALSE(thing.ok());
  EXPECT_EQ(Status::UNKNOWN, thing.status());
}

TEST(StatusOr, TestPointerStatusCtor) {
  StatusOr<int*> thing(Status::CANCELLED);
  EXPECT_FALSE(thing.ok());
  EXPECT_EQ(Status::CANCELLED, thing.status());
}

TEST(StatusOr, TestPointerValueCtor) {
  const int kI = 4;
  StatusOr<const int*> thing(&kI);
  EXPECT_TRUE(thing.ok());
  EXPECT_EQ(&kI, thing.ValueOrDie());
}

TEST(StatusOr, TestPointerCopyCtorStatusOk) {
  const int kI = 0;
  StatusOr<const int*> original(&kI);
  StatusOr<const int*> copy(original);
  EXPECT_EQ(original.status(), copy.status());
  EXPECT_EQ(original.ValueOrDie(), copy.ValueOrDie());
}

TEST(StatusOr, TestPointerCopyCtorStatusNotOk) {
  StatusOr<int*> original(Status::CANCELLED);
  StatusOr<int*> copy(original);
  EXPECT_EQ(original.status(), copy.status());
}

TEST(StatusOr, TestPointerCopyCtorStatusOKConverting) {
  Derived derived;
  StatusOr<Derived*> original(&derived);
  StatusOr<Base2*> copy(original);
  EXPECT_EQ(original.status(), copy.status());
  EXPECT_EQ(static_cast<const Base2*>(original.ValueOrDie()),
            copy.ValueOrDie());
}

TEST(StatusOr, TestPointerCopyCtorStatusNotOkConverting) {
  StatusOr<Derived*> original(Status::CANCELLED);
  StatusOr<Base2*> copy(original);
  EXPECT_EQ(original.status(), copy.status());
}

TEST(StatusOr, TestPointerAssignmentStatusOk) {
  const int kI = 0;
  StatusOr<const int*> source(&kI);
  StatusOr<const int*> target;
  target = source;
  EXPECT_EQ(source.status(), target.status());
  EXPECT_EQ(source.ValueOrDie(), target.ValueOrDie());
}

TEST(StatusOr, TestPointerAssignmentStatusNotOk) {
  StatusOr<int*> source(Status::CANCELLED);
  StatusOr<int*> target;
  target = source;
  EXPECT_EQ(source.status(), target.status());
}

TEST(StatusOr, TestPointerAssignmentStatusOKConverting) {
  Derived derived;
  StatusOr<Derived*> source(&derived);
  StatusOr<Base2*> target;
  target = source;
  EXPECT_EQ(source.status(), target.status());
  EXPECT_EQ(static_cast<const Base2*>(source.ValueOrDie()),
            target.ValueOrDie());
}

TEST(StatusOr, TestPointerAssignmentStatusNotOkConverting) {
  StatusOr<Derived*> source(Status::CANCELLED);
  StatusOr<Base2*> target;
  target = source;
  EXPECT_EQ(source.status(), target.status());
}

TEST(StatusOr, TestPointerStatus) {
  const int kI = 0;
  StatusOr<const int*> good(&kI);
  EXPECT_TRUE(good.ok());
  StatusOr<const int*> bad(Status::CANCELLED);
  EXPECT_EQ(Status::CANCELLED, bad.status());
}

TEST(StatusOr, TestPointerValue) {
  const int kI = 0;
  StatusOr<const int*> thing(&kI);
  EXPECT_EQ(&kI, thing.ValueOrDie());
}

TEST(StatusOr, TestPointerValueConst) {
  const int kI = 0;
  const StatusOr<const int*> thing(&kI);
  EXPECT_EQ(&kI, thing.ValueOrDie());
}

}  // namespace
}  // namespace bolt

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}