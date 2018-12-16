#ifndef UTIL_MEMFILE_MEMFILE_H
#define UTIL_MEMFILE_MEMFILE_H

#include <cstdint>
#include <string>
#include "hoist/macros.h"
#include "hoist/status.h"
#include "hoist/statusor.h"

namespace util {
namespace memfile {

using ::Hoist::Status;
using ::Hoist::StatusOr;
using ::std::string;

class MemFile final {
 public:
  static StatusOr<MemFile*> OpenMemFile(string path, size_t size);

  explicit MemFile(string path)
      : path_(path), fd_(kNoFd), size_(0), data_(nullptr) {}

  // Resize will resize the file to the exact size.
  Status Resize(size_t size);

  // ResizeMinimum will resize the file to at least the specified size.
  Status ResizeMinimum(size_t size);

  // Size gets the size of the allocated data.
  size_t Size() const { return size_; }

  // Data gets a pointer to the allocated data.
  void* Data() const { return data_; }

  // DataAt gets a pointer to the data at an offset.
  void* DataAt(size_t offset) const {
    return static_cast<void*>(static_cast<char*>(data_) + offset);
  }

  // Sync will sync the memory file to disk.
  Status Sync();

  // Sync range will sync a range of memory to disk.
  Status SyncRange(size_t offset, size_t length);

  // Close closes the MemFile and it should no longer be used.
  Status Close();

  // Path gets the file path used to open the file.
  const string Path() const { return path_; }

  ~MemFile();

 private:
  // kNoFd is the sentinel value meaning no file descriptor has been assigned to
  // the MemFile yet.
  static const int kNoFd = -1;

  // path_ is the file path.
  string path_;
  // fd_ is the file desriptor of the open file.
  int fd_;
  // size_ is the mmaped file size.
  size_t size_;
  // data_ is
  void* data_;

  // FileSize gets the size of the file on disk. The file must already exist.
  StatusOr<size_t> FileSize();
  // ReopenFile ensures that the file is open before mmapping.
  Status ReopenFile();
  // MmapFd performs the mmap operator to link the file to memory.
  Status MmapFd(size_t size);
  // Truncate performs the resizing of a file to the specified size.
  Status Truncate(off_t size);

  DISALLOW_COPY_AND_ASSIGN(MemFile);
};

}  // namespace memfile
}  // namespace util

#endif  // UTIL_MEMFILE_MEMFILE_H