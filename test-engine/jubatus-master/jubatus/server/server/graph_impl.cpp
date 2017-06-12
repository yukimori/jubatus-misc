// This file is auto-generated from graph.idl(0.6.4-33-gcc8d7ca) with jenerator version 0.8.5-6-g5a2c923/develop
// *** DO NOT EDIT ***

#include <map>
#include <string>
#include <vector>
#include <utility>
#include "jubatus/util/lang/shared_ptr.h"

#include "../../server/framework.hpp"
#include "graph_serv.hpp"

namespace jubatus {
namespace server {

class graph_impl : public jubatus::server::common::mprpc::rpc_server {
 public:
  explicit graph_impl(const jubatus::server::framework::server_argv& a):
    rpc_server(a.timeout),
    p_(new jubatus::server::framework::server_helper<graph_serv>(a, true)) {

    rpc_server::add<std::string(std::string)>("create_node",
        jubatus::util::lang::bind(&graph_impl::create_node, this));
    rpc_server::add<bool(std::string, std::string)>("remove_node",
        jubatus::util::lang::bind(&graph_impl::remove_node, this,
        jubatus::util::lang::_2));
    rpc_server::add<bool(std::string, std::string, std::map<std::string,
        std::string>)>("update_node", jubatus::util::lang::bind(
        &graph_impl::update_node, this, jubatus::util::lang::_2,
        jubatus::util::lang::_3));
    rpc_server::add<uint64_t(std::string, std::string, edge)>("create_edge",
        jubatus::util::lang::bind(&graph_impl::create_edge, this,
        jubatus::util::lang::_2, jubatus::util::lang::_3));
    rpc_server::add<bool(std::string, std::string, uint64_t, edge)>(
        "update_edge", jubatus::util::lang::bind(&graph_impl::update_edge, this,
        jubatus::util::lang::_2, jubatus::util::lang::_3,
        jubatus::util::lang::_4));
    rpc_server::add<bool(std::string, std::string, uint64_t)>("remove_edge",
        jubatus::util::lang::bind(&graph_impl::remove_edge, this,
        jubatus::util::lang::_2, jubatus::util::lang::_3));
    rpc_server::add<double(std::string, std::string, int32_t,
        jubatus::core::graph::preset_query)>("get_centrality",
        jubatus::util::lang::bind(&graph_impl::get_centrality, this,
        jubatus::util::lang::_2, jubatus::util::lang::_3,
        jubatus::util::lang::_4));
    rpc_server::add<bool(std::string, jubatus::core::graph::preset_query)>(
        "add_centrality_query", jubatus::util::lang::bind(
        &graph_impl::add_centrality_query, this, jubatus::util::lang::_2));
    rpc_server::add<bool(std::string, jubatus::core::graph::preset_query)>(
        "add_shortest_path_query", jubatus::util::lang::bind(
        &graph_impl::add_shortest_path_query, this, jubatus::util::lang::_2));
    rpc_server::add<bool(std::string, jubatus::core::graph::preset_query)>(
        "remove_centrality_query", jubatus::util::lang::bind(
        &graph_impl::remove_centrality_query, this, jubatus::util::lang::_2));
    rpc_server::add<bool(std::string, jubatus::core::graph::preset_query)>(
        "remove_shortest_path_query", jubatus::util::lang::bind(
        &graph_impl::remove_shortest_path_query, this,
        jubatus::util::lang::_2));
    rpc_server::add<std::vector<std::string>(std::string, shortest_path_query)>(
        "get_shortest_path", jubatus::util::lang::bind(
        &graph_impl::get_shortest_path, this, jubatus::util::lang::_2));
    rpc_server::add<bool(std::string)>("update_index",
        jubatus::util::lang::bind(&graph_impl::update_index, this));
    rpc_server::add<bool(std::string)>("clear", jubatus::util::lang::bind(
        &graph_impl::clear, this));
    rpc_server::add<jubatus::core::graph::node_info(std::string, std::string)>(
        "get_node", jubatus::util::lang::bind(&graph_impl::get_node, this,
        jubatus::util::lang::_2));
    rpc_server::add<edge(std::string, std::string, uint64_t)>("get_edge",
        jubatus::util::lang::bind(&graph_impl::get_edge, this,
        jubatus::util::lang::_2, jubatus::util::lang::_3));
    rpc_server::add<bool(std::string, std::string)>("create_node_here",
        jubatus::util::lang::bind(&graph_impl::create_node_here, this,
        jubatus::util::lang::_2));
    rpc_server::add<bool(std::string, std::string)>("remove_global_node",
        jubatus::util::lang::bind(&graph_impl::remove_global_node, this,
        jubatus::util::lang::_2));
    rpc_server::add<bool(std::string, uint64_t, edge)>("create_edge_here",
        jubatus::util::lang::bind(&graph_impl::create_edge_here, this,
        jubatus::util::lang::_2, jubatus::util::lang::_3));

    rpc_server::add<std::string(std::string)>("get_config",
        jubatus::util::lang::bind(&graph_impl::get_config, this));
    rpc_server::add<std::map<std::string, std::string>(std::string,
        std::string)>("save", jubatus::util::lang::bind(&graph_impl::save, this,
        jubatus::util::lang::_2));
    rpc_server::add<bool(std::string, std::string)>("load",
        jubatus::util::lang::bind(&graph_impl::load, this,
        jubatus::util::lang::_2));
    rpc_server::add<std::map<std::string, std::map<std::string, std::string> >(
        std::string)>("get_status", jubatus::util::lang::bind(
        &graph_impl::get_status, this));
  }

  std::string create_node() {
    NOLOCK_(p_);
    return get_p()->create_node();
  }

  bool remove_node(const std::string& node_id) {
    NOLOCK_(p_);
    return get_p()->remove_node(node_id);
  }

  bool update_node(const std::string& node_id, const std::map<std::string,
      std::string>& property) {
    JWLOCK_(p_);
    return get_p()->update_node(node_id, property);
  }

  uint64_t create_edge(const std::string& node_id, const edge& e) {
    NOLOCK_(p_);
    return get_p()->create_edge(node_id, e);
  }

  bool update_edge(const std::string& node_id, uint64_t edge_id,
      const edge& e) {
    JWLOCK_(p_);
    return get_p()->update_edge(node_id, edge_id, e);
  }

  bool remove_edge(const std::string& node_id, uint64_t edge_id) {
    JWLOCK_(p_);
    return get_p()->remove_edge(node_id, edge_id);
  }

  double get_centrality(const std::string& node_id, int32_t centrality_type,
      const jubatus::core::graph::preset_query& query) {
    JRLOCK_(p_);
    return get_p()->get_centrality(node_id, centrality_type, query);
  }

  bool add_centrality_query(const jubatus::core::graph::preset_query& query) {
    JWLOCK_(p_);
    return get_p()->add_centrality_query(query);
  }

  bool add_shortest_path_query(
      const jubatus::core::graph::preset_query& query) {
    JWLOCK_(p_);
    return get_p()->add_shortest_path_query(query);
  }

  bool remove_centrality_query(
      const jubatus::core::graph::preset_query& query) {
    JWLOCK_(p_);
    return get_p()->remove_centrality_query(query);
  }

  bool remove_shortest_path_query(
      const jubatus::core::graph::preset_query& query) {
    JWLOCK_(p_);
    return get_p()->remove_shortest_path_query(query);
  }

  std::vector<std::string> get_shortest_path(const shortest_path_query& query) {
    JRLOCK_(p_);
    return get_p()->get_shortest_path(query);
  }

  bool update_index() {
    JWLOCK_(p_);
    return get_p()->update_index();
  }

  bool clear() {
    JWLOCK_(p_);
    return get_p()->clear();
  }

  jubatus::core::graph::node_info get_node(const std::string& node_id) {
    JRLOCK_(p_);
    return get_p()->get_node(node_id);
  }

  edge get_edge(const std::string& node_id, uint64_t edge_id) {
    JRLOCK_(p_);
    return get_p()->get_edge(node_id, edge_id);
  }

  bool create_node_here(const std::string& node_id) {
    JWLOCK_(p_);
    return get_p()->create_node_here(node_id);
  }

  bool remove_global_node(const std::string& node_id) {
    JWLOCK_(p_);
    return get_p()->remove_global_node(node_id);
  }

  bool create_edge_here(uint64_t edge_id, const edge& e) {
    JWLOCK_(p_);
    return get_p()->create_edge_here(edge_id, e);
  }

  std::string get_config() {
    JRLOCK_(p_);
    return get_p()->get_config();
  }

  std::map<std::string, std::string> save(const std::string& id) {
    JRLOCK_(p_);
    return get_p()->save(id);
  }

  bool load(const std::string& id) {
    JWLOCK_(p_);
    return get_p()->load(id);
  }

  std::map<std::string, std::map<std::string, std::string> > get_status() {
    JRLOCK_(p_);
    return p_->get_status();
  }

  int run() { return p_->start(*this); }
  jubatus::util::lang::shared_ptr<graph_serv> get_p() { return p_->server(); }

 private:
  jubatus::util::lang::shared_ptr<jubatus::server::framework::server_helper<graph_serv> > p_;
};

}  // namespace server
}  // namespace jubatus

int main(int argc, char* argv[]) {
  return
    jubatus::server::framework::run_server<jubatus::server::graph_impl,
        jubatus::server::graph_serv>
      (argc, argv, "graph");
}
