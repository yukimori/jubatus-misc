// This file is auto-generated from kvs.idl with jenerator version /master
// *** DO NOT EDIT ***

#ifndef KVS_TYPES_HPP_
#define KVS_TYPES_HPP_

#include <stdint.h>

#include <map>
#include <string>
#include <vector>
#include <utility>

#include <jubatus/core/fv_converter/datum.hpp>
#include <msgpack.hpp>

namespace jubatus {

struct entry {
 public:
  MSGPACK_DEFINE(key, value);
  std::string key;
  int32_t value;
  entry() {
  }
  entry(const std::string& key, int32_t value)
    : key(key), value(value) {
  }
};

}  // namespace jubatus

#endif  // KVS_TYPES_HPP_
