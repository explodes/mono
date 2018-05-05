#include "service.h"
#include <sys/time.h>
#include "memory.h"

namespace statusz {

grpc::Status StatuszService::Poll(grpc::ServerContext* context,
                                  const commonpb::Empty* request,
                                  Status* response) {
  MemoryUsage mem = collectMemoryStatistics();
  response->mutable_memory()->set_process_memory(mem.process);
  response->mutable_memory()->set_system_memory(mem.system);
  response->mutable_memory()->set_system_total(mem.total);

  unsigned long long timestamp = time(NULL);
  response->set_timestamp(timestamp);

  return grpc::Status::OK;
}

}  // namespace statusz