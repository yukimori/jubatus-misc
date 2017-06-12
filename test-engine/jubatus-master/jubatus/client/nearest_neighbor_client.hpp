// This file is auto-generated from nearest_neighbor.idl(0.8.2-20-g8e4dc3b) with jenerator version 0.8.5-6-g5a2c923/feature/improve-get_labels-ulong
// *** DO NOT EDIT ***

#ifndef JUBATUS_CLIENT_NEAREST_NEIGHBOR_CLIENT_HPP_
#define JUBATUS_CLIENT_NEAREST_NEIGHBOR_CLIENT_HPP_

#include <map>
#include <string>
#include <vector>
#include <jubatus/client/common/client.hpp>
#include <jubatus/client/common/datum.hpp>
#include "nearest_neighbor_types.hpp"

namespace jubatus {
namespace nearest_neighbor {
namespace client {

class nearest_neighbor : public jubatus::client::common::client {
 public:
  nearest_neighbor(const std::string& host, uint64_t port,
      const std::string& name, unsigned int timeout_sec)
      : client(host, port, name, timeout_sec) {
  }

  bool clear() {
    msgpack::rpc::future f = c_.call("clear", name_);
    return f.get<bool>();
  }

  bool set_row(const std::string& id, const jubatus::client::common::datum& d) {
    msgpack::rpc::future f = c_.call("set_row", name_, id, d);
    return f.get<bool>();
  }

  std::vector<id_with_score> neighbor_row_from_id(const std::string& id,
      uint32_t size) {
    msgpack::rpc::future f = c_.call("neighbor_row_from_id", name_, id, size);
    return f.get<std::vector<id_with_score> >();
  }

  std::vector<id_with_score> neighbor_row_from_datum(
      const jubatus::client::common::datum& query, uint32_t size) {
    msgpack::rpc::future f = c_.call("neighbor_row_from_datum", name_, query,
        size);
    return f.get<std::vector<id_with_score> >();
  }

  std::vector<id_with_score> similar_row_from_id(const std::string& id,
      uint32_t ret_num) {
    msgpack::rpc::future f = c_.call("similar_row_from_id", name_, id, ret_num);
    return f.get<std::vector<id_with_score> >();
  }

  std::vector<id_with_score> similar_row_from_datum(
      const jubatus::client::common::datum& query, uint32_t ret_num) {
    msgpack::rpc::future f = c_.call("similar_row_from_datum", name_, query,
        ret_num);
    return f.get<std::vector<id_with_score> >();
  }

  std::vector<std::string> get_all_rows() {
    msgpack::rpc::future f = c_.call("get_all_rows", name_);
    return f.get<std::vector<std::string> >();
  }
};

}  // namespace client
}  // namespace nearest_neighbor
}  // namespace jubatus

#endif  // JUBATUS_CLIENT_NEAREST_NEIGHBOR_CLIENT_HPP_
