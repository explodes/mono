#include "client.h"
#include "hoist/logging.h"

namespace statusz {

Hoist::Result<std::shared_ptr<Status>> StatuszClient::Poll() {
  grpc::ClientContext context;
  std::shared_ptr<Status> result = std::make_shared<Status>();

  grpc::Status status = stub_->Poll(&context, commonpb::Empty(), result.get());

  if (status.ok()) {
    return Hoist::Result<std::shared_ptr<Status>>(result);
  } else {
    ELOG("poll statusz error " << status.error_code()
                               << " msg: " << status.error_message());
    return Hoist::Result<std::shared_ptr<Status>>(
        Hoist::StatusCode::UnknownError);
  }
}

}  // namespace statusz