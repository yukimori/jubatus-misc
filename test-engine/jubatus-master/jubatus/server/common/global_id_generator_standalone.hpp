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

#ifndef JUBATUS_SERVER_COMMON_GLOBAL_ID_GENERATOR_STANDALONE_HPP_
#define JUBATUS_SERVER_COMMON_GLOBAL_ID_GENERATOR_STANDALONE_HPP_

#include <stdint.h>
#include <string>

#include "jubatus/util/lang/scoped_ptr.h"

#include "global_id_generator_base.hpp"

namespace jubatus {
namespace server {
namespace common {

class global_id_generator_standalone: public global_id_generator_base {
  struct impl;

 public:
  global_id_generator_standalone();
  explicit global_id_generator_standalone(uint64_t counter);
  virtual ~global_id_generator_standalone();

  virtual uint64_t generate();

 private:
  jubatus::util::lang::scoped_ptr<impl> pimpl_;
};

}  // namespace common
}  // namespace server
}  // namespace jubatus

#endif  // JUBATUS_SERVER_COMMON_GLOBAL_ID_GENERATOR_STANDALONE_HPP_
