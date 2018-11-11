#include "net/spacefight/service.h"

#include <unistd.h>
#include <chrono>
#include <string>
#include <thread>
#include "hoist/logging.h"
#include "hoist/math.h"
#include "net/spacefight/elements.h"

namespace spacefight {

grpc::Status SpacefightService::Login(grpc::ServerContext* context,
                                      const Registration* request,
                                      Token* response) {
  DLOG("register " << request->username());

  char token[65];
  for (int i = 0; i < 64; i++) {
    token[i] = Hoist::RNG::rand<char>('a', 'z' + 1);
  }
  token[64] = '\0';
  std::string tokens(token);
  DLOG("create token " << tokens);

  PlayerInput input;
  input.set_token(tokens);
  input.set_username(request->username());
  int64_t player_id = game_.createNewPlayer(&input);

  response->set_token(tokens);
  response->set_player_id(player_id);

  return grpc::Status::OK;
};

grpc::Status SpacefightService::Update(
    grpc::ServerContext* context,
    grpc::ServerReaderWriter<World, PlayerInput>* stream) {
  // ok is true while either the read/write connection succeeds
  bool ok = true;

  PlayerInput input;

  // receive input updates
  std::thread input_thread([this, &context, &stream, &ok, &input]() {
    while (ok) {
      if (context->IsCancelled() || !stream->Read(&input)) {
        ok = false;
        DLOG("read ended");
        return;
      }
      game_.apply(&input);
      if (input.quit()) {
        ok = false;
        DLOG("client quit");
        return;
      }
    }
  });

  // stream world status updates
  World world;
  while (ok) {
    game_.getWorld(&world);
    if (context->IsCancelled() || !stream->Write(world)) {
      ok = false;
      DLOG("write ended");
      break;
    }
    std::this_thread::sleep_for(settings::world_update_interval);
  }

  input_thread.join();

  input.set_quit(true);
  if (!input.token().empty()) {
    game_.apply(&input);
  }

  return grpc::Status::OK;
}

}  // namespace spacefight