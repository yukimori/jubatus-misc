// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011,2012 Preferred Networks and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include "graph_serv.hpp"

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "jubatus/util/concurrent/lock.h"
#include "jubatus/util/text/json.h"
#include "jubatus/util/system/time_util.h"
#include "jubatus/util/lang/shared_ptr.h"

#include "jubatus/core/common/assert.hpp"
#include "jubatus/core/common/vector_util.hpp"
#include "jubatus/core/common/jsonconfig.hpp"
#include "jubatus/core/graph/graph_factory.hpp"

#ifdef HAVE_ZOOKEEPER_H
#include "../common/cht.hpp"
#endif
#include "../common/global_id_generator_standalone.hpp"
#include "../common/logger/logger.hpp"
#include "../common/unique_lock.hpp"
#ifdef HAVE_ZOOKEEPER_H
#include "../common/global_id_generator_zk.hpp"
#include "../common/membership.hpp"
#include "../common/mprpc/rpc_mclient.hpp"
#endif
#ifdef HAVE_ZOOKEEPER_H
#include "../framework/aggregators.hpp"
#endif
#include "../framework/mixer/mixer_factory.hpp"
#include "graph_client.hpp"

using std::string;
using std::vector;
using std::pair;
using jubatus::util::lang::lexical_cast;
using jubatus::util::text::json::json;
using jubatus::core::graph::preset_query;
using jubatus::core::graph::node_info;
using jubatus::server::common::lock_service;
using jubatus::server::framework::server_argv;
using jubatus::server::framework::mixer::create_mixer;

using jubatus::util::system::time::clock_time;
using jubatus::util::system::time::get_clock_time;

namespace jubatus {
namespace server {

enum graph_serv_error {
  NODE_ALREADY_EXISTS = 0xDEADBEEF
};

namespace {

struct graph_serv_config {
  std::string method;
  // TODO(oda): we should use optional<jsonconfig::config> instead of
  //            jsonconfig::config ?
  core::common::jsonconfig::config parameter;

  template<typename Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(method) & JUBA_MEMBER(parameter);
  }
};

inline node_id uint642nodeid(uint64_t i) {
  return jubatus::util::lang::lexical_cast<node_id, uint64_t>(i);
}

inline uint64_t nodeid2uint64(const node_id& id) {
  return jubatus::util::lang::lexical_cast<uint64_t, node_id>(id);
}

inline node_id i2n(uint64_t i) {
  return uint642nodeid(i);
}

inline uint64_t n2i(const node_id& id) {
  return nodeid2uint64(id);
}

}  // namespace

graph_serv::graph_serv(
    const framework::server_argv& a,
    const jubatus::util::lang::shared_ptr<lock_service>& zk)
    : server_base(a),
      mixer_(create_mixer(a, zk, rw_mutex(), user_data_version())) {

#ifdef HAVE_ZOOKEEPER_H
  if (a.is_standalone()) {
#endif
    idgen_.reset(new common::global_id_generator_standalone());
#ifdef HAVE_ZOOKEEPER_H
  } else {
    zk_ = zk;

    common::global_id_generator_zk* idgen_zk =
        new common::global_id_generator_zk();
    idgen_.reset(idgen_zk);

    std::string counter_path;
    common::build_actor_path(counter_path, a.type, a.name);
    idgen_zk->set_ls(zk_, counter_path);
  }
#endif
}

graph_serv::~graph_serv() {
}

void graph_serv::set_config(const std::string& config) {
  core::common::jsonconfig::config conf_root(
      lexical_cast<jubatus::util::text::json::json>(config));
  graph_serv_config conf =
    core::common::jsonconfig::config_cast_check<graph_serv_config>(conf_root);

  config_ = config;

#if 0
  // TODO(oda): we should use optional<jsonconfig::config> instead of
  //            jsonconfig::config ?

  core::common::jsonconfig::config param;
  if (conf.parameter) {
    param = *conf.parameter;
  }
#endif

  graph_.reset(
      new core::driver::graph(
          core::graph::graph_factory::create_graph(
              conf.method, conf.parameter)));
  mixer_->set_driver(graph_.get());

  LOG(INFO) << "config loaded: " << config;
}

std::string graph_serv::get_config() const {
  check_set_config();
  return config_;
}

void graph_serv::check_set_config() const {
  if (!graph_) {
    throw JUBATUS_EXCEPTION(core::common::config_not_set());
  }
}

void graph_serv::get_status(status_t& status) const {
  check_set_config();

  status_t my_status;
  graph_->get_model()->get_status(my_status);
  status.insert(my_status.begin(), my_status.end());
}

uint64_t graph_serv::user_data_version() const {
  return 1;  // should be inclemented when model data is modified
}

std::string graph_serv::create_node() { /* no lock here */
  check_set_config();

  uint64_t nid = idgen_->generate();
  std::string nid_str = jubatus::util::lang::lexical_cast<std::string>(nid);

#ifdef HAVE_ZOOKEEPER_H
  if (!argv().is_standalone()) {
    // we dont need global locking, because getting unique id from zk
    // guarantees there'll be no data confliction
    {
      std::vector<std::pair<std::string, int> > nodes;
      find_from_cht_(nid_str, 2, nodes);
      if (nodes.empty()) {
        throw JUBATUS_EXCEPTION(
            jubatus::core::common::exception::runtime_error(
                "no server found in cht: " + argv().name));
      }
      try {
        selective_create_node_(nodes[0], nid_str);
      } catch (const std::runtime_error& e) {
        throw JUBATUS_EXCEPTION(core::common::exception::runtime_error(
            "failed to create node " + nid_str + " (" + e.what() + "): " +
            nodes[0].first + ":" + lexical_cast<std::string>(nodes[0].second)));
      }

      for (size_t i = 1; i < nodes.size(); ++i) {
        try {
          selective_create_node_(nodes[i], nid_str);
        } catch (const core::graph::local_node_exists& e) {  // pass through
        } catch (const core::graph::global_node_exists& e) {  // pass through
        } catch (const std::runtime_error& e) {  // error !
          LOG(WARNING) << "cannot create " << i << "th replica "
                       << "(" << e.what() << "): "
                       << nodes[i].first << ":" << nodes[i].second;
        }
      }
    }
  } else {
#endif
    jubatus::util::concurrent::scoped_wlock write_lk(rw_mutex());
    graph_->create_node(nid);
#ifdef HAVE_ZOOKEEPER_H
  }
#endif
  DLOG(INFO) << "node created: " << nid_str;
  return nid_str;
}

bool graph_serv::update_node(
    const std::string& id,
    const std::map<std::string, std::string>& p) {
  check_set_config();

  graph_->update_node(n2i(id), p);
  DLOG(INFO) << "node updated: " << id;
  return true;
}

bool graph_serv::remove_node(const std::string& nid_str) {
  // locks manually because we should unlock before global access
  // make sure this function not to be called from other functions
  server::common::unique_wlock lk(rw_mutex());
  event_model_updated();

  check_set_config();

  core::graph::node_id_t nid = n2i(nid_str);
  graph_->remove_node(nid);

#ifdef HAVE_ZOOKEEPER_H
  if (!argv().is_standalone()) {
    // send true remove_node_ to other machine,
    // if conflicts with create_node, users should re-run to ensure removal
    std::vector<std::pair<std::string, int> > members;
    get_members_(members);

    if (!members.empty()) {
      // create global node
      common::mprpc::rpc_mclient c(members, argv().interconnect_timeout);

#ifndef NDEBUG
      for (size_t i = 0; i < members.size(); i++) {
        DLOG(INFO) << "request to "
            << members[i].first << ":" << members[i].second;
      }
#endif

      try {
        // requires unlock before global access to prevent dead-lock
        lk.unlock();

        c.call("remove_global_node",
               argv().name,
               nid_str,
               jubatus::util::lang::function<int(int, int)>(
                   &jubatus::server::framework::add<int>));
      } catch (const common::mprpc::rpc_no_result& e) {  // pass through
        DLOG(INFO) << e.diagnostic_information(true);
      }
    }
  }
#endif
  DLOG(INFO) << "node removed: " << nid_str;
  return true;
}

// @cht
edge_id_t graph_serv::create_edge(const std::string& id, const edge& ei) {
  check_set_config();

  edge_id_t eid = idgen_->generate();
  // TODO(kuenishi): assert id==ei.src

#ifdef HAVE_ZOOKEEPER_H
  if (!argv().is_standalone()) {
    // we dont need global locking, because getting unique id from zk
    // guarantees there'll be no data confliction
    std::vector<std::pair<std::string, int> > nodes;
    find_from_cht_(ei.source, 2, nodes);
    if (nodes.empty()) {
      throw JUBATUS_EXCEPTION(
          jubatus::core::common::exception::runtime_error(
              "no server found in cht: " + argv().name));
    }
    // TODO(kuenishi): assertion: nodes[0] should be myself
    {
      jubatus::util::concurrent::scoped_wlock wirte_lk(rw_mutex());
      this->create_edge_here(eid, ei);
    }
    for (size_t i = 1; i < nodes.size(); ++i) {
      try {
        if (nodes[i].first == argv().eth && nodes[i].second == argv().port) {
        } else {
          client::graph c(nodes[i].first,
                          nodes[i].second,
                          argv().name,
                          argv().interconnect_timeout);
          DLOG(INFO) << "request to "
              << nodes[i].first << ":" << nodes[i].second;
          c.create_edge_here(eid, ei);
        }
      } catch (const core::graph::local_node_exists& e) {  // pass through
      } catch (const core::graph::global_node_exists& e) {  // pass through
      } catch (const std::runtime_error& e) {  // error !
        LOG(WARNING) << "cannot create " << i << "th replica "
                     << "(" << e.what() << "): "
                     << nodes[i].first << ":" << nodes[i].second;
      }
    }
  } else {
#endif
    jubatus::util::concurrent::scoped_wlock write_lk(rw_mutex());
    graph_->create_edge(eid, n2i(ei.source), n2i(ei.target), ei.property);
#ifdef HAVE_ZOOKEEPER_H
  }
#endif

  DLOG(INFO) << "edge created: " << eid
      << " ( " << ei.source << " => " << ei.target << " )";
  return eid;
}

// @random
bool graph_serv::update_edge(
    const std::string&,
    edge_id_t eid,
    const edge& ei) {
  check_set_config();

  graph_->update_edge(eid, ei.property);
  DLOG(INFO) << "edge updated: " << eid
      << " ( " << ei.source << " => " << ei.target << " )";
  return true;
}

bool graph_serv::remove_edge(const std::string&, const edge_id_t& id) {
  check_set_config();

  graph_->remove_edge(id);
  DLOG(INFO) << "edge removed: " << id;
  return true;
}

// @random
double graph_serv::get_centrality(
    const std::string& id,
    const centrality_type& s,
    const preset_query& q) const {
  check_set_config();

  if (s == 0) {
    return graph_->get_centrality(n2i(id), core::graph::EIGENSCORE, q);
  } else {
    std::stringstream msg;
    msg << "unknown centrality type: " << s;
    throw JUBATUS_EXCEPTION(core::common::exception::runtime_error(msg.str()));
  }
}

// @random
std::vector<node_id> graph_serv::get_shortest_path(
    const shortest_path_query& req) const {
  std::vector<core::graph::node_id_t> ret0 =
      graph_->get_shortest_path(
          n2i(req.source),
          n2i(req.target),
          req.max_hop,
          req.query);
  std::vector<node_id> ret;
  for (size_t i = 0; i < ret0.size(); ++i) {
    ret.push_back(i2n(ret0[i]));
  }
  return ret;
}

// update, broadcast
bool graph_serv::add_centrality_query(const preset_query& q) {
  check_set_config();

  graph_->add_centrality_query(q);
  DLOG(INFO) << "centrality query added";
  return true;
}

// update, broadcast
bool graph_serv::add_shortest_path_query(const preset_query& q) {
  check_set_config();

  graph_->add_shortest_path_query(q);
  DLOG(INFO) << "shortest path added";
  return true;
}

// update, broadcast
bool graph_serv::remove_centrality_query(const preset_query& q) {
  check_set_config();

  graph_->remove_centrality_query(q);
  DLOG(INFO) << "centrality query removed";
  return true;
}

// update, broadcast
bool graph_serv::remove_shortest_path_query(const preset_query& q) {
  check_set_config();

  graph_->remove_shortest_path_query(q);
  DLOG(INFO) << "shortest path removed";
  return true;
}

node_info graph_serv::get_node(const std::string& nid) const {
  check_set_config();

  return graph_->get_node(n2i(nid));
}
// @random
edge graph_serv::get_edge(
    const std::string& nid,
    edge_id_t eid) const {
  check_set_config();

  core::graph::edge_info info = graph_->get_edge(eid);
  jubatus::edge ret;
  ret.property = info.p;
  ret.source = i2n(info.src);
  ret.target = i2n(info.tgt);
  return ret;
}

// @broadcast
bool graph_serv::update_index() {
#ifdef HAVE_ZOOKEEPER_H
  if (!argv().is_standalone()) {
    throw JUBATUS_EXCEPTION(
        jubatus::core::common::exception::runtime_error(
            "manual mix is available only in standalone mode."));
  }
#endif
  check_set_config();

  clock_time start = get_clock_time();
  graph_->update_index();
  clock_time end = get_clock_time();
  LOG(INFO)
      << "mix done manually and locally in "
      << static_cast<double>(end - start) << " secs.";
  return true;
}

bool graph_serv::clear() {
  check_set_config();

  graph_->clear();

  LOG(INFO) << "model cleared: " << argv().name;
  return true;
}

bool graph_serv::create_node_here(const std::string& nid) {
  graph_->create_node_here(n2i(nid));
  return true;
}

bool graph_serv::remove_global_node(const std::string& nid) {
  graph_->remove_global_node(n2i(nid));

  DLOG(INFO) << "global node removed";
  return true;
}  // update internal

bool graph_serv::create_edge_here(edge_id_t eid, const edge& ei) {
  graph_->create_edge_here(eid, n2i(ei.source), n2i(ei.target), ei.property);
  return true;
}

/*
 * Updates the model on the specified node.
 * If the host/port of the current process is specified, update
 * is processed locally.
 * Caller is responsible for handling exceptions including RPC
 * errors.
 */
void graph_serv::selective_create_node_(
    const std::pair<std::string, int>& target,
    const std::string nid_str) {
  if (target.first == argv().eth && target.second == argv().port) {
    jubatus::util::concurrent::scoped_wlock write_lk(rw_mutex());
    this->create_node_here(nid_str);
  } else {
    // must not lock here
    client::graph c(target.first,
                    target.second,
                    argv().name,
                    argv().interconnect_timeout);
    c.create_node_here(nid_str);
  }
}

void graph_serv::find_from_cht_(
    const std::string& key,
    size_t n,
    std::vector<std::pair<std::string, int> >& out) {
  out.clear();
#ifdef HAVE_ZOOKEEPER_H
  common::cht ht(zk_, argv().type, argv().name);
  ht.find(key, out, n);  // replication number of local_node
#else
  // cannot reach here, assertion!
  JUBATUS_ASSERT_UNREACHABLE();
  // JUBATUS_ASSERT(argv().is_standalone());
  // out.push_back(make_pair(argv().eth, argv().port));
#endif
}

void graph_serv::get_members_(std::vector<std::pair<std::string, int> >& ret) {
  ret.clear();
#ifdef HAVE_ZOOKEEPER_H
  common::get_all_nodes(*zk_, argv().type, argv().name, ret);

  if (ret.empty()) {
    return;
  }
  try {
    // remove myself
    for (std::vector<std::pair<std::string, int> >::iterator it = ret.begin();
        it != ret.end(); it++) {
      if (it->first == argv().eth && it->second == argv().port) {
        it = ret.erase(it);
        it--;
        continue;
      }
    }
  } catch(...) {
    // eliminate the exception "no clients."
  }
#endif
}

bool graph_serv::load(const std::string& id) {
  if (server_base::load(id)) {
    reset_id_generator();
    return true;
  }
  return false;
}

void graph_serv::load_file(const std::string& path) {
  server_base::load_file(path);
  reset_id_generator();
}

void graph_serv::reset_id_generator() {
  if (argv().is_standalone()) {
    uint64_t counter = graph_->find_max_int_id() + 1;
    idgen_.reset(
        new common::global_id_generator_standalone(counter));
  } else {
    // TODO(kmaehashi): support ID check for distributed mode
  }
}

}  // namespace server
}  // namespace jubatus
