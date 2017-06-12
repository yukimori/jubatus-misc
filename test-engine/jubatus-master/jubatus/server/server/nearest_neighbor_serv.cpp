// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011-2013 Preferred Networks and Nippon Telegraph and Telephone Corporation.
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

#include "nearest_neighbor_serv.hpp"

#include <string>
#include <vector>
#include "jubatus/util/concurrent/lock.h"
#include "jubatus/util/lang/cast.h"
#include "jubatus/util/text/json.h"
#include "jubatus/core/common/exception.hpp"
#include "jubatus/core/common/jsonconfig.hpp"
#include "jubatus/core/storage/column_table.hpp"
#include "jubatus/core/fv_converter/converter_config.hpp"
#include "jubatus/core/fv_converter/datum.hpp"
#include "jubatus/core/fv_converter/revert.hpp"
#include "jubatus/core/nearest_neighbor/nearest_neighbor_factory.hpp"
#include "../common/membership.hpp"
#include "../common/logger/logger.hpp"
#include "../framework/mixer/mixer_factory.hpp"

using std::string;
using jubatus::util::lang::lexical_cast;
using jubatus::util::lang::shared_ptr;
using jubatus::core::fv_converter::datum;
using jubatus::server::framework::mixer::create_mixer;

namespace jubatus {
namespace server {
namespace {

struct nearest_neighbor_serv_config {
  std::string method;
  jubatus::util::data::optional<core::common::jsonconfig::config> parameter;
  core::fv_converter::converter_config converter;

  template<typename Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(method) & JUBA_MEMBER(parameter) & JUBA_MEMBER(converter);
  }
};

}  // namespace

nearest_neighbor_serv::nearest_neighbor_serv(
    const framework::server_argv& a,
    const shared_ptr<common::lock_service>& zk)
    : server_base(a),
      mixer_(create_mixer(a, zk, rw_mutex(), user_data_version())),
      update_row_cnt_(0) {
}

nearest_neighbor_serv::~nearest_neighbor_serv() {
}

void nearest_neighbor_serv::get_status(status_t& status) const {
  status_t my_status;
  my_status["update_row_cnt"] = lexical_cast<string>(update_row_cnt_);
  my_status["data"] = lexical_cast<string>(
      nearest_neighbor_->get_table()->dump_json());
  status.insert(my_status.begin(), my_status.end());
}

uint64_t nearest_neighbor_serv::user_data_version() const {
  return 1;  // should be inclemented when model data is modified
}

void nearest_neighbor_serv::set_config(const std::string& config) {
  core::common::jsonconfig::config config_root(
      lexical_cast<jubatus::util::text::json::json>(config));
  nearest_neighbor_serv_config conf =
    core::common::jsonconfig::config_cast_check<nearest_neighbor_serv_config>(
        config_root);

  config_ = config;

  core::common::jsonconfig::config param;
  if (conf.parameter) {
    param = *conf.parameter;
  }

  DLOG(INFO) << __func__;
  shared_ptr<core::fv_converter::datum_to_fv_converter> converter =
      core::fv_converter::make_fv_converter(conf.converter, &so_loader_);

  shared_ptr<core::storage::column_table>
      table(new core::storage::column_table);
  std::string my_id;
#ifdef HAVE_ZOOKEEPER_H
  my_id = common::build_loc_str(argv().eth, argv().port);
#endif

  shared_ptr<jubatus::core::nearest_neighbor::nearest_neighbor_base>
      nn(jubatus::core::nearest_neighbor::create_nearest_neighbor(
          conf.method, param, table, my_id));
  nearest_neighbor_.reset(new core::driver::nearest_neighbor(nn, converter));
  mixer_->set_driver(nearest_neighbor_.get());

  LOG(INFO) << "config loaded: " << config;
}

std::string nearest_neighbor_serv::get_config() const {
  DLOG(INFO) << __func__;
  check_set_config();
  return config_;
}

bool nearest_neighbor_serv::clear() {
  DLOG(INFO) << __func__;
  check_set_config();
  update_row_cnt_ = 0;
  nearest_neighbor_->clear();
  return true;
}

bool nearest_neighbor_serv::set_row(const std::string& id, const datum& d) {
  // DLOG(INFO) << __func__ << " " << id;
  check_set_config();

  ++update_row_cnt_;
  nearest_neighbor_->set_row(id, d);
  return true;
}

neighbor_result nearest_neighbor_serv::neighbor_row_from_id(
    const std::string& id,
    size_t size) {
  DLOG(INFO) << __func__;
  check_set_config();

  return nearest_neighbor_->neighbor_row_from_id(id, size);
}

neighbor_result nearest_neighbor_serv::neighbor_row_from_datum(
    const datum& d,
    size_t size) {
  // DLOG(INFO) << __func__;
  check_set_config();

  return nearest_neighbor_->neighbor_row_from_datum(d, size);
}

neighbor_result nearest_neighbor_serv::similar_row_from_id(
    const std::string& id,
    size_t ret_num) {
  DLOG(INFO) << __func__;
  check_set_config();

  return nearest_neighbor_->similar_row(id, ret_num);
}

neighbor_result nearest_neighbor_serv::similar_row_from_datum(
    const datum& d,
    size_t ret_num) {
  DLOG(INFO) << __func__;
  check_set_config();

  return nearest_neighbor_->similar_row(d, ret_num);
}

std::vector<std::string> nearest_neighbor_serv::get_all_rows() const {
  check_set_config();

  return nearest_neighbor_->get_all_rows();
}

void nearest_neighbor_serv::check_set_config() const {
  if (!nearest_neighbor_) {
    throw JUBATUS_EXCEPTION(core::common::config_not_set());
  }
}

}  // namespace server
}  // namespace jubatus
