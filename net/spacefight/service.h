#ifndef NET_SPACEFIGHT_SERVICE_H
#define NET_SPACEFIGHT_SERVICE_H

#include "net/spacefight/game.h"
#include "proto/spacefight/spacefight.pb.h"
#include "proto/spacefight/spacefight_service.grpc.pb.h"

namespace spacefight {

class SpacefightService final : public Spacefight::Service {
 public:
  SpacefightService(Game& game) : game_(game) {}

  ::grpc::Status Login(::grpc::ServerContext* context,
                       const Registration* request, Token* response) override;
  ::grpc::Status Update(
      ::grpc::ServerContext* context,
      ::grpc::ServerReaderWriter<World, PlayerInput>* stream) override;

 private:
  Game& game_;
};

}  // namespace spacefight

#endif