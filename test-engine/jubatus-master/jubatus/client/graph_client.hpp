// This file is auto-generated from graph.idl(0.6.4-33-gcc8d7ca) with jenerator version 0.8.5-6-g5a2c923/feature/improve-get_labels-ulong
// *** DO NOT EDIT ***

#ifndef JUBATUS_CLIENT_GRAPH_CLIENT_HPP_
#define JUBATUS_CLIENT_GRAPH_CLIENT_HPP_

#include <map>
#include <string>
#include <vector>
#include <jubatus/client/common/client.hpp>
#include <jubatus/client/common/datum.hpp>
#include "graph_types.hpp"

namespace jubatus {
namespace graph {
namespace client {

class graph : public jubatus::client::common::client {
 public:
  graph(const std::string& host, uint64_t port, const std::string& name,
      unsigned int timeout_sec)
      : client(host, port, name, timeout_sec) {
  }

  std::string create_node() {
    msgpack::rpc::future f = c_.call("create_node", name_);
    return f.get<std::string>();
  }

  bool remove_node(const std::string& node_id) {
    msgpack::rpc::future f = c_.call("remove_node", name_, node_id);
    return f.get<bool>();
  }

  bool update_node(const std::string& node_id, const std::map<std::string,
      std::string>& property) {
    msgpack::rpc::future f = c_.call("update_node", name_, node_id, property);
    return f.get<bool>();
  }

  uint64_t create_edge(const std::string& node_id, const edge& e) {
    msgpack::rpc::future f = c_.call("create_edge", name_, node_id, e);
    return f.get<uint64_t>();
  }

  bool update_edge(const std::string& node_id, uint64_t edge_id,
      const edge& e) {
    msgpack::rpc::future f = c_.call("update_edge", name_, node_id, edge_id, e);
    return f.get<bool>();
  }

  bool remove_edge(const std::string& node_id, uint64_t edge_id) {
    msgpack::rpc::future f = c_.call("remove_edge", name_, node_id, edge_id);
    return f.get<bool>();
  }

  double get_centrality(const std::string& node_id, int32_t centrality_type,
      const preset_query& query) {
    msgpack::rpc::future f = c_.call("get_centrality", name_, node_id,
        centrality_type, query);
    return f.get<double>();
  }

  bool add_centrality_query(const preset_query& query) {
    msgpack::rpc::future f = c_.call("add_centrality_query", name_, query);
    return f.get<bool>();
  }

  bool add_shortest_path_query(const preset_query& query) {
    msgpack::rpc::future f = c_.call("add_shortest_path_query", name_, query);
    return f.get<bool>();
  }

  bool remove_centrality_query(const preset_query& query) {
    msgpack::rpc::future f = c_.call("remove_centrality_query", name_, query);
    return f.get<bool>();
  }

  bool remove_shortest_path_query(const preset_query& query) {
    msgpack::rpc::future f = c_.call("remove_shortest_path_query", name_,
        query);
    return f.get<bool>();
  }

  std::vector<std::string> get_shortest_path(const shortest_path_query& query) {
    msgpack::rpc::future f = c_.call("get_shortest_path", name_, query);
    return f.get<std::vector<std::string> >();
  }

  bool update_index() {
    msgpack::rpc::future f = c_.call("update_index", name_);
    return f.get<bool>();
  }

  bool clear() {
    msgpack::rpc::future f = c_.call("clear", name_);
    return f.get<bool>();
  }

  node get_node(const std::string& node_id) {
    msgpack::rpc::future f = c_.call("get_node", name_, node_id);
    return f.get<node>();
  }

  edge get_edge(const std::string& node_id, uint64_t edge_id) {
    msgpack::rpc::future f = c_.call("get_edge", name_, node_id, edge_id);
    return f.get<edge>();
  }

  bool create_node_here(const std::string& node_id) {
    msgpack::rpc::future f = c_.call("create_node_here", name_, node_id);
    return f.get<bool>();
  }

  bool remove_global_node(const std::string& node_id) {
    msgpack::rpc::future f = c_.call("remove_global_node", name_, node_id);
    return f.get<bool>();
  }

  bool create_edge_here(uint64_t edge_id, const edge& e) {
    msgpack::rpc::future f = c_.call("create_edge_here", name_, edge_id, e);
    return f.get<bool>();
  }
};

}  // namespace client
}  // namespace graph
}  // namespace jubatus

#endif  // JUBATUS_CLIENT_GRAPH_CLIENT_HPP_
