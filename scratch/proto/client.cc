#include "webs/statusz/client.h"
#include <google/protobuf/text_format.h>
#include <grpc++/grpc++.h>
#include <iostream>
#include <string>
#include "hoist/hoist.h"
#include "scratch/proto/sample.pb.h"
#include "scratch/proto/sample_service.grpc.pb.h"
#include "scratch/proto/sample_service.pb.h"

void dumpMessage(const google::protobuf::Message *const message) {
  if (!message) {
    std::cout << "Message null" << std::endl;
    return;
  }
  std::string s;
  google::protobuf::TextFormat::PrintToString(*message, &s);
  std::cout << s << std::endl;
}

void dumpStatus(const grpc::Status status) {
  if (status.ok()) {
    std::cout << "Status OK!" << std::endl;
  } else {
    std::cout << "status ERROR " << status.error_code()
              << " :: " << status.error_message() << std::endl;
  }
}

class ContentClient final {
 public:
  explicit ContentClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(scratch::ContentService::NewStub(channel)), statusz_(channel) {}

  grpc::Status Get(scratch::ContentResponse *response) {
    grpc::ClientContext context;
    const scratch::ContentRequest request;

    return stub_->Get(&context, request, response);
  }

  Hoist::Result<std::vector<scratch::Content *>> Stream() {
    grpc::ClientContext context;
    const scratch::ContentRequest request;
    std::unique_ptr<grpc::ClientReader<scratch::Content>> stream(
        stub_->Streaming(&context, request));

    std::vector<scratch::Content *> contents;
    while (true) {
      scratch::Content *content = new scratch::Content();
      if (!stream->Read(content)) {
        break;
      }
      contents.push_back(content);
    }
    grpc::Status status = stream->Finish();

    if (!status.ok()) {
      return Hoist::Result<std::vector<scratch::Content *>>(
          Hoist::StatusCode::UnknownError);
    } else {
      return Hoist::Result<std::vector<scratch::Content *>>(contents);
    }
  }

  void printStatusz() {
    auto statusz = statusz_.Poll();
    if (statusz.ok()) {
      dumpMessage(statusz.value().get());
    }
  }

 private:
  std::unique_ptr<scratch::ContentService::Stub> stub_;
  statusz::StatuszClient statusz_;
};

void doGet(ContentClient &client) {
  std::cout << "GET" << std::endl;
  scratch::ContentResponse response;
  grpc::Status status = client.Get(&response);

  dumpStatus(status);
  if (status.ok()) {
    dumpMessage(&response);
  }
}

void doStream(ContentClient &client) {
  std::cout << "STREAM" << std::endl;
  Hoist::Result<std::vector<scratch::Content *>> results(client.Stream());

  if (!results.ok()) {
    std::cout << "FAILURE!!" << std::endl;
  } else {
    for (auto content : results.value()) {
      dumpMessage(content);
      delete content;
    }
  }
}

void init() {
  Hoist::Init();
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  // Hoist::waitForInput(
  //    "Press enter to continue or after debugger has attached...");
}

void shutdown() { google::protobuf::ShutdownProtobufLibrary(); }

int main() {
  init();

  ContentClient client(
      grpc::CreateChannel("0.0.0.0:50052", grpc::InsecureChannelCredentials()));

  doGet(client);
  doStream(client);
  client.printStatusz();

  shutdown();

  return 0;
}