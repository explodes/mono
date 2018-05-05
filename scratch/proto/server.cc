#include <grpc++/grpc++.h>
#include <iostream>
#include <string>
#include "scratch/proto/sample.pb.h"
#include "scratch/proto/sample_service.grpc.pb.h"
#include "scratch/proto/sample_service.pb.h"
#include "webs/statusz/service.h"

class ContentServiceImpl : public scratch::ContentService::Service {
 public:
  ::grpc::Status Get(::grpc::ServerContext* context,
                     const scratch::ContentRequest* request,
                     scratch::ContentResponse* response) override {
    for (int i = 0; i < 2; i++) {
      scratch::Content* content = response->add_contents();
      content->set_ordinal(i);
      content->set_message("Hello, world!");
    }
    std::cout << "filled \"Get\" content " << std::endl;
    return grpc::Status::OK;
  }

  ::grpc::Status Streaming(
      ::grpc::ServerContext* context, const scratch::ContentRequest* request,
      ::grpc::ServerWriter<scratch::Content>* writer) override {
    scratch::Content content;
    for (int i = 0; i < 2; i++) {
      std::cout << "\"Stream\" content " << i << std::endl;
      content.set_ordinal(i);
      content.set_message("Hello, world!");
      writer->Write(content);
    }
    return grpc::Status::OK;
  }
};

void createAndRunGrpcServer() {
  std::string server_address("0.0.0.0:50052");
  ContentServiceImpl service;
  statusz::StatuszService statusz;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  builder.RegisterService(&statusz);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  createAndRunGrpcServer();

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}