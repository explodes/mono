#ifndef NET_STATUSZ_SERVICE_H
#define NET_STATUSZ_SERVICE_H

#include "proto/common/empty.pb.h"
#include "proto/statusz/statusz.pb.h"
#include "proto/statusz/statusz_service.grpc.pb.h"

namespace statusz {

class StatuszService final : public Statusz::Service {
 public:
  ::grpc::Status Poll(::grpc::ServerContext* context,
                      const commonpb::Empty* request,
                      Status* response) override;
};

}  // namespace statusz

#endif