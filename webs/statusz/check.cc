// Get a print the statusz of a server.
// usage:
//  ./check host:999
#include <google/protobuf/text_format.h>
#include <grpc++/grpc++.h>
#include <memory>
#include "hoist/init.h"
#include "hoist/logging.h"
#include "hoist/statusor.h"
#include "webs/statusz/client.h"

Hoist::StatusOr<std::shared_ptr<statusz::Status>> poll(std::string &addr) {
  std::shared_ptr<grpc::Channel> channel =
      grpc::CreateChannel(addr, grpc::InsecureChannelCredentials());
  statusz::StatuszClient client(channel);
  return client.Poll();
}

int main(int argc, char *argv[]) {
  Hoist::Init();
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    std::cout << "Usage: \n" << argv[0] << " host:port" << std::endl;
    return 1;
  }

  std::string addr(argv[1]);

  ILOG("STATUSZ");
  ILOG("Checking server " << addr);

  Hoist::StatusOr<std::shared_ptr<statusz::Status>> status = poll(addr);

  if (!status.ok()) {
    ELOG("Error checking " << addr << '.');
  } else {
    std::string s;
    google::protobuf::TextFormat::PrintToString(*status.ValueOrDie(), &s);
    ILOG("Got STATUSZ\n" << s);
  }

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}