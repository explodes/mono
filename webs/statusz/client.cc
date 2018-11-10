#include "webs/statusz/client.h"

#include <grpc++/grpc++.h>
#include "hoist/logging.h"
#include "hoist/status.h"
#include "hoist/statusor.h"

namespace statusz {

Hoist::StatusOr<std::shared_ptr<Status>> StatuszClient::Poll() {
  grpc::ClientContext context;
  std::shared_ptr<Status> result = std::make_shared<Status>();

  grpc::Status status = stub_->Poll(&context, commonpb::Empty(), result.get());

  if (status.ok()) {
    return result;
  } else {
    ELOG("poll statusz error " << status.error_code()
                               << " msg: " << status.error_message());
    return Hoist::Status(Hoist::error::UNKNOWN, status.error_message());
  }
}

}  // namespace statusz