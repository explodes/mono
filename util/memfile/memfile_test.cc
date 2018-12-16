#include "util/memfile/memfile.h"

#include <unistd.h>
#include "gtest/gtest.h"
#include "hoist/status.h"
#include "hoist/status_macros.h"

namespace util {
namespace memfile {
namespace {

using Hoist::Status;

static const string kPath = "/tmp/foo";

class MemFileTest : public ::testing::Test {
 protected:
  void SetUp() override { CreateFile(); }

  void CreateFile() {
    auto result = MemFile::OpenMemFile(kPath, 10);
    ASSERT_OK(result);
    file_ = result.ValueOrDie();
  }

  void CloseFile() {
    EXPECT_OK(file_->Close());
    delete file_;
    file_ = nullptr;
  }

  void DeleteFile() {
    Status status = Status::OK;
    errno = 0;
    if (unlink(kPath.c_str())) {
      status = ERRNO_AS_STATUS();
    }
    EXPECT_OK(status);
  }

  void TearDown() override {
    CloseFile();
    DeleteFile();
  }

  MemFile* file_;
};

TEST_F(MemFileTest, WriteFile) {
  char* s = reinterpret_cast<char*>(file_->Data());
  s[0] = 'a';
  s[1] = 'b';
  s[2] = 'c';
  EXPECT_OK(file_->Sync());

  CloseFile();
  CreateFile();

  s = reinterpret_cast<char*>(file_->Data());
  EXPECT_EQ(s[0], 'a');
  EXPECT_EQ(s[1], 'b');
  EXPECT_EQ(s[2], 'c');
}

}  // namespace
}  // namespace memfile
}  // namespace util

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}