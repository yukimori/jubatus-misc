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

#include "stat_serv.hpp"

#include <algorithm>
#include <string>

#include "jubatus/util/lang/shared_ptr.h"
#include "jubatus/util/text/json.h"

#include "jubatus/core/common/jsonconfig.hpp"
#include "../framework/mixer/mixer_factory.hpp"

using std::string;
using std::make_pair;
using jubatus::util::text::json::json;
using jubatus::util::lang::lexical_cast;

using jubatus::server::common::lock_service;
using jubatus::server::framework::server_argv;
using jubatus::server::framework::mixer::create_mixer;

namespace jubatus {
namespace server {

struct stat_serv_config {
  int32_t window_size;

  template<typename Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(window_size);
  }
};

stat_serv::stat_serv(const server_argv& a,
                     const jubatus::util::lang::shared_ptr<lock_service>& zk)
    : server_base(a),
      mixer_(create_mixer(a, zk, rw_mutex(), user_data_version())) {
}

stat_serv::~stat_serv() {
}

void stat_serv::get_status(status_t& status) const {
  status.insert(make_pair("storage", stat_->get_model()->type()));
}

uint64_t stat_serv::user_data_version() const {
  return 1;  // should be inclemented when model data is modified
}

void stat_serv::set_config(const string& config) {
  core::common::jsonconfig::config conf_root(lexical_cast<json>(config));
  stat_serv_config conf =
      core::common::jsonconfig::config_cast_check<stat_serv_config>(conf_root);

  config_ = config;
  stat_.reset(
      new core::driver::stat(new core::stat::stat(conf.window_size)));
  mixer_->set_driver(stat_.get());

  LOG(INFO) << "config loaded: " << config;
}

string stat_serv::get_config() const {
  return config_;
}

bool stat_serv::push(const std::string& key, double value) {
  stat_->push(key, value);
  DLOG(INFO) << "pushed: " << key;
  return true;
}

double stat_serv::sum(const std::string& key) const {
  return stat_->sum(key);
}

double stat_serv::stddev(const std::string& key) const {
  return stat_->stddev(key);
}

double stat_serv::max(const std::string& key) const {
  return stat_->max(key);
}

double stat_serv::min(const std::string& key) const {
  return stat_->min(key);
}

double stat_serv::entropy(const std::string& key) const {
  return stat_->entropy();
}

double stat_serv::moment(const std::string& key, int n, double c) const {
  return stat_->moment(key, n, c);
}

bool stat_serv::clear() {
  stat_->get_model()->clear();
  LOG(INFO) << "model cleared: " << argv().name;
  return true;
}

}  // namespace server
}  // namespace jubatus
