#include "util/memfile/memfile.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "hoist/status_macros.h"

namespace util {
namespace memfile {

namespace error = ::Hoist::error;

StatusOr<MemFile*> MemFile::OpenMemFile(string path, size_t size) {
  MemFile* mem_file = new MemFile(path);
  // Resize to at least our target size.
  RETURN_IF_ERROR(mem_file->ResizeMinimum(size));
  return mem_file;
}

Status MemFile::Resize(size_t size) {
  RETURN_IF_ERROR(ReopenFile());
  RETURN_IF_ERROR(Truncate(size));
  RETURN_IF_ERROR(MmapFd(size));
  return Status::OK;
}

Status MemFile::ResizeMinimum(size_t size) {
  RETURN_IF_ERROR(ReopenFile());

  ASSIGN_OR_RETURN(size_t file_size, FileSize());
  if (size > file_size) {
    // The file is too small, resize it.
    RETURN_IF_ERROR(Truncate(size));
  }
  // Map the results.
  RETURN_IF_ERROR(MmapFd(size));

  return Status::OK;
}

Status MemFile::Sync() { return SyncRange(0, size_); }

Status MemFile::SyncRange(size_t offset, size_t length) {
  errno = 0;
  if (msync(DataAt(offset), length, MS_SYNC)) {
    return ERRNO_AS_STATUS();
  }
  return Status::OK;
}

Status MemFile::Close() {
  Status status = Status::OK;

  // Close the open file.
  if (fd_ != kNoFd) {
    errno = 0;
    if (close(fd_)) {
      status = ERRNO_AS_STATUS();
    };
    fd_ = kNoFd;
  }

  // Unmap the memory.
  if (data_ != nullptr) {
    errno = 0;
    if (munmap(data_, size_)) {
      status = ERRNO_AS_STATUS();
    }
    data_ = nullptr;
  }

  // Our data should be considered empty.
  size_ = 0;

  return status;
}

StatusOr<size_t> MemFile::FileSize() {
  struct stat stats;
  errno = 0;
  if (fstat(fd_, &stats)) {
    return ERRNO_AS_STATUS();
  }
  return stats.st_size;
}

Status MemFile::ReopenFile() {
  // Close the file if it might already be open.
  RETURN_IF_ERROR(Close());

  // Open the file.
  errno = 0;
  int fd = open(path_.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    return ERRNO_AS_STATUS();
  }

  // Save our file descriptor.
  fd_ = fd;

  return Status::OK;
}

Status MemFile::MmapFd(size_t size) {
  // Map our file to memory.
  errno = 0;
  void* addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
  if (addr == MAP_FAILED) {
    return ERRNO_AS_STATUS();
  }
  // Save the mapped location.
  data_ = addr;
  size_ = size;

  // Advise that the memory will be read/write at random locations.
  errno = 0;
  if (madvise(addr, size, MADV_RANDOM)) {
    Close();
    return ERRNO_AS_STATUS();
  }

  return Status::OK;
}

Status MemFile::Truncate(off_t size) {
  // Truncate the file.
  errno = 0;
  if (ftruncate(fd_, size)) {
    return ERRNO_AS_STATUS();
  }
  // Save the new size.
  size_ = size;

  return Status::OK;
}

MemFile::~MemFile() { Close(); }

}  // namespace memfile
}  // namespace util