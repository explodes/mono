#ifndef WEBS_STATUSZ_CLIENT_H
#define WEBS_STATUSZ_CLIENT_H

#include <grpc++/grpc++.h>
#include <memory>
#include "hoist/statusor.h"
#include "proto/common/empty.pb.h"
#include "proto/statusz/statusz.pb.h"
#include "proto/statusz/statusz_service.grpc.pb.h"

namespace statusz {

class StatuszClient final {
 public:
  explicit StatuszClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(Statusz::NewStub(channel)) {}

  Hoist::StatusOr<std::shared_ptr<Status>> Poll();

 private:
  std::unique_ptr<Statusz::Stub> stub_;
};

}  // namespace statusz

#endif