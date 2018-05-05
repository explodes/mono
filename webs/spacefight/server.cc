#include <grpc++/grpc++.h>
#include "hoist/init.h"
#include "hoist/logging.h"
#include "webs/spacefight/game.h"
#include "webs/spacefight/service.h"
#include "webs/statusz/service.h"

void createAndRunSpacefight(spacefight::Game &game) {
  std::string server_address("0.0.0.0:50099");
  spacefight::SpacefightService service(game);
  statusz::StatuszService statusz;

  ILOG("Initializing server at " << server_address);

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  builder.RegisterService(&statusz);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  ILOG("Server listening on " << server_address);
  ILOG(" - spacefight enabled");
  ILOG(" - statusz enabled");

  server->Wait();
}

int main() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  Hoist::Init();

  spacefight::Game game;

  DLOG("Initializing game...");
  game.start();
  DLOG("Game initialized.");

  createAndRunSpacefight(game);

  DLOG("Ending game...");
  game.end();
  DLOG("Game over.");

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}