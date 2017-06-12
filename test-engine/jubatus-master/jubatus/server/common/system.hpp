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

#ifndef JUBATUS_SERVER_COMMON_SYSTEM_HPP_
#define JUBATUS_SERVER_COMMON_SYSTEM_HPP_

#include <stdint.h>

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace jubatus {
namespace server {
namespace common {

struct machine_status_t {
  uint64_t vm_size;  // VIRT
  uint64_t vm_resident;  // RSS
  uint64_t vm_share;  // SHR
};

std::string get_program_name();
std::string get_user_name();

int daemonize();

void append_env_path(const std::string& env_, const std::string& argv0);
void append_server_path(const std::string& argv0);

void get_machine_status(machine_status_t& status);

}  // namespace common
}  // namespace server
}  // namespace jubatus

#endif  // JUBATUS_SERVER_COMMON_SYSTEM_HPP_
