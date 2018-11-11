#include "net/spacefight/debug.h"

namespace spacefight {
namespace debug {

void dumpMessage(const google::protobuf::Message* const message) {
  IF_NDEBUG(return );
  if (!message) {
    std::cout << "Message null" << std::endl;
    return;
  }
  std::string s;
  google::protobuf::TextFormat::PrintToString(*message, &s);
  std::cout << s << std::endl;
}

}  // namespace debug
}  // namespace spacefight