// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2013 Preferred Networks and Nippon Telegraph and Telephone Corporation.
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

#ifndef JUBATUS_SERVER_SERVER_CLUSTERING_SERV_HPP_
#define JUBATUS_SERVER_SERVER_CLUSTERING_SERV_HPP_

#include <string>
#include <utility>
#include <vector>
#include "jubatus/core/driver/clustering.hpp"
#include "jubatus/core/clustering/types.hpp"
#include "jubatus/core/fv_converter/so_factory.hpp"
#include "../framework/server_base.hpp"
#include "clustering_types.hpp"

namespace jubatus {
namespace server {

class clustering_serv : public framework::server_base {
 public:
  clustering_serv(
      const framework::server_argv& a,
      const jubatus::util::lang::shared_ptr<common::lock_service>& zk);
  virtual ~clustering_serv();

  framework::mixer::mixer* get_mixer() const {
    return mixer_.get();
  }

  core::driver::driver_base* get_driver() const {
    return clustering_.get();
  }

  void get_status(status_t& status) const;
  uint64_t user_data_version() const;

  void set_config(const std::string& config);
  std::string get_config() const;

  bool push(const std::vector<core::clustering::indexed_point>& points);

  core::fv_converter::datum get_nearest_center(
      const core::fv_converter::datum& point) const;
  std::vector<std::pair<double, core::fv_converter::datum> >
      get_nearest_members(const core::fv_converter::datum& point) const;
  std::vector<std::pair<double, std::string> >
      get_nearest_members_light(const core::fv_converter::datum& point) const;

  std::vector<core::fv_converter::datum> get_k_center() const;
  std::vector<std::vector<std::pair<double, core::fv_converter::datum> > >
      get_core_members() const;
  std::vector<std::vector<std::pair<double, std::string> > >
      get_core_members_light() const;

  size_t get_revision() const;

  void check_set_config() const;

 private:
  jubatus::util::lang::shared_ptr<framework::mixer::mixer> mixer_;
  jubatus::util::lang::shared_ptr<core::driver::clustering> clustering_;
  std::string config_;
  jubatus::core::fv_converter::so_factory so_loader_;
};

}  // namespace server
}  // namespace jubatus

#endif  // JUBATUS_SERVER_SERVER_CLUSTERING_SERV_HPP_
