// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011 Preferred Networks and Nippon Telegraph and Telephone Corporation.
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

#ifndef JUBATUS_SERVER_COMMON_MEMBERSHIP_HPP_
#define JUBATUS_SERVER_COMMON_MEMBERSHIP_HPP_

#include <string>
#include <utility>
#include <vector>
#include <map>
#include "jubatus/util/lang/function.h"

#include "lock_service.hpp"

namespace jubatus {
namespace server {
namespace common {

static const std::string JUBATUS_BASE_PATH = "/jubatus";
static const std::string JUBAVISOR_BASE_PATH = "/jubatus/supervisors";
static const std::string JUBAPROXY_BASE_PATH = "/jubatus/jubaproxies";
static const std::string ACTOR_BASE_PATH = "/jubatus/actors";
static const std::string CONFIG_BASE_PATH = "/jubatus/config";

// "127.0.0.1" -> 9199 -> "127.0.0.1_9199"
std::string build_loc_str(const std::string&, int, unsigned int = 0);

// /path/base -> 127.0.0.1 -> 9199 -> /path/base/127.0.0.1_9199
void build_existence_path(
    const std::string&,
    const std::string&,
    int,
    std::string&);

void build_actor_path(
    std::string&,
    const std::string& type,
    const std::string& name);

void build_config_path(
    std::string&,
    const std::string& type,
    const std::string& name);

void build_config_lock_path(
    std::string&,
    const std::string&,
    const std::string&);

// 127.0.0.1_9199 -> (127.0.0.1, 9199)
bool revert(const std::string&, std::string&, int&);

// zk -> name -> ip -> port -> void
void register_actor(
    lock_service&,
    const std::string& type,
    const std::string& name,
    const std::string& ip,
    int port);

void register_active(
    lock_service& z,
    const std::string& type,
    const std::string& name,
    const std::string& ip,
    int port);

void unregister_active(
    lock_service& z,
    const std::string& type,
    const std::string& name,
    const std::string& ip,
    int port);


void watch_delete_actor(
    lock_service& z,
    const std::string& type,
    const std::string& name,
    const std::string& ip,
    int port,
    jubatus::util::lang::function<void(std::string)> callback);

// zk -> name -> ip -> port -> void
void register_proxy(
    lock_service&,
    const std::string& type,
    const std::string& ip,
    int);

// zk -> name -> list( (ip, rpc_port) )
bool get_all_nodes(
    lock_service&,
    const std::string& type,
    const std::string& name,
    std::vector<std::pair<std::string, int> >&);

void shutdown_server();

void prepare_jubatus(
    lock_service& ls,
    const std::string& type,
    const std::string& name = "");

}  // namespace common
}  // namespace server
}  // namespace jubatus

#endif  // JUBATUS_SERVER_COMMON_MEMBERSHIP_HPP_
