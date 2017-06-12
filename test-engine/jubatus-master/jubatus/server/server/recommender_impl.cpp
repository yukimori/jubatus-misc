// This file is auto-generated from recommender.idl(0.6.4-33-gcc8d7ca) with jenerator version 0.8.5-6-g5a2c923/develop
// *** DO NOT EDIT ***

#include <map>
#include <string>
#include <vector>
#include <utility>
#include "jubatus/util/lang/shared_ptr.h"

#include "../../server/framework.hpp"
#include "recommender_serv.hpp"

namespace jubatus {
namespace server {

class recommender_impl : public jubatus::server::common::mprpc::rpc_server {
 public:
  explicit recommender_impl(const jubatus::server::framework::server_argv& a):
    rpc_server(a.timeout),
    p_(new jubatus::server::framework::server_helper<recommender_serv>(a,
        true)) {

    rpc_server::add<bool(std::string, std::string)>("clear_row",
        jubatus::util::lang::bind(&recommender_impl::clear_row, this,
        jubatus::util::lang::_2));
    rpc_server::add<bool(std::string, std::string,
        jubatus::core::fv_converter::datum)>("update_row",
        jubatus::util::lang::bind(&recommender_impl::update_row, this,
        jubatus::util::lang::_2, jubatus::util::lang::_3));
    rpc_server::add<bool(std::string)>("clear", jubatus::util::lang::bind(
        &recommender_impl::clear, this));
    rpc_server::add<jubatus::core::fv_converter::datum(std::string,
        std::string)>("complete_row_from_id", jubatus::util::lang::bind(
        &recommender_impl::complete_row_from_id, this,
        jubatus::util::lang::_2));
    rpc_server::add<jubatus::core::fv_converter::datum(std::string,
        jubatus::core::fv_converter::datum)>("complete_row_from_datum",
        jubatus::util::lang::bind(&recommender_impl::complete_row_from_datum,
        this, jubatus::util::lang::_2));
    rpc_server::add<std::vector<id_with_score>(std::string, std::string,
        uint32_t)>("similar_row_from_id", jubatus::util::lang::bind(
        &recommender_impl::similar_row_from_id, this, jubatus::util::lang::_2,
        jubatus::util::lang::_3));
    rpc_server::add<std::vector<id_with_score>(std::string,
        jubatus::core::fv_converter::datum, uint32_t)>("similar_row_from_datum",
        jubatus::util::lang::bind(&recommender_impl::similar_row_from_datum,
        this, jubatus::util::lang::_2, jubatus::util::lang::_3));
    rpc_server::add<jubatus::core::fv_converter::datum(std::string,
        std::string)>("decode_row", jubatus::util::lang::bind(
        &recommender_impl::decode_row, this, jubatus::util::lang::_2));
    rpc_server::add<std::vector<std::string>(std::string)>("get_all_rows",
        jubatus::util::lang::bind(&recommender_impl::get_all_rows, this));
    rpc_server::add<float(std::string, jubatus::core::fv_converter::datum,
        jubatus::core::fv_converter::datum)>("calc_similarity",
        jubatus::util::lang::bind(&recommender_impl::calc_similarity, this,
        jubatus::util::lang::_2, jubatus::util::lang::_3));
    rpc_server::add<float(std::string, jubatus::core::fv_converter::datum)>(
        "calc_l2norm", jubatus::util::lang::bind(&recommender_impl::calc_l2norm,
        this, jubatus::util::lang::_2));

    rpc_server::add<std::string(std::string)>("get_config",
        jubatus::util::lang::bind(&recommender_impl::get_config, this));
    rpc_server::add<std::map<std::string, std::string>(std::string,
        std::string)>("save", jubatus::util::lang::bind(&recommender_impl::save,
        this, jubatus::util::lang::_2));
    rpc_server::add<bool(std::string, std::string)>("load",
        jubatus::util::lang::bind(&recommender_impl::load, this,
        jubatus::util::lang::_2));
    rpc_server::add<std::map<std::string, std::map<std::string, std::string> >(
        std::string)>("get_status", jubatus::util::lang::bind(
        &recommender_impl::get_status, this));
  }

  bool clear_row(const std::string& id) {
    JWLOCK_(p_);
    return get_p()->clear_row(id);
  }

  bool update_row(const std::string& id,
      const jubatus::core::fv_converter::datum& row) {
    JWLOCK_(p_);
    return get_p()->update_row(id, row);
  }

  bool clear() {
    JWLOCK_(p_);
    return get_p()->clear();
  }

  jubatus::core::fv_converter::datum complete_row_from_id(
      const std::string& id) {
    JRLOCK_(p_);
    return get_p()->complete_row_from_id(id);
  }

  jubatus::core::fv_converter::datum complete_row_from_datum(
      const jubatus::core::fv_converter::datum& row) {
    JRLOCK_(p_);
    return get_p()->complete_row_from_datum(row);
  }

  std::vector<id_with_score> similar_row_from_id(const std::string& id,
      uint32_t size) {
    JRLOCK_(p_);
    return get_p()->similar_row_from_id(id, size);
  }

  std::vector<id_with_score> similar_row_from_datum(
      const jubatus::core::fv_converter::datum& row, uint32_t size) {
    JRLOCK_(p_);
    return get_p()->similar_row_from_datum(row, size);
  }

  jubatus::core::fv_converter::datum decode_row(const std::string& id) {
    JRLOCK_(p_);
    return get_p()->decode_row(id);
  }

  std::vector<std::string> get_all_rows() {
    JRLOCK_(p_);
    return get_p()->get_all_rows();
  }

  float calc_similarity(const jubatus::core::fv_converter::datum& lhs,
      const jubatus::core::fv_converter::datum& rhs) {
    JRLOCK_(p_);
    return get_p()->calc_similarity(lhs, rhs);
  }

  float calc_l2norm(const jubatus::core::fv_converter::datum& row) {
    JRLOCK_(p_);
    return get_p()->calc_l2norm(row);
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
  jubatus::util::lang::shared_ptr<recommender_serv> get_p() { return p_->server(
      ); }

 private:
  jubatus::util::lang::shared_ptr<jubatus::server::framework::server_helper<recommender_serv> > p_;
};

}  // namespace server
}  // namespace jubatus

int main(int argc, char* argv[]) {
  return
    jubatus::server::framework::run_server<jubatus::server::recommender_impl,
        jubatus::server::recommender_serv>
      (argc, argv, "recommender");
}
