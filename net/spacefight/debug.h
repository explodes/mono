#ifndef NET_SPACEFIGHT_DEBUG_H
#define NET_SPACEFIGHT_DEBUG_H

#include <google/protobuf/text_format.h>
#include "hoist/logging.h"

namespace spacefight {
namespace debug {

void dumpMessage(const google::protobuf::Message* const message);

}  // namespace debug
}  // namespace spacefight

#endif