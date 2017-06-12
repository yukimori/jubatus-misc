#ifndef KVS_DRIVER_HPP_
#define KVS_DRIVER_HPP_

#include <stdint.h>

#include <jubatus/core/driver/driver.hpp>

#include "kvs_algorithm.hpp"

namespace jubatus {
namespace core {
namespace driver {

class kvs_driver : public jubatus::core::driver::driver_base {
public:
  kvs_driver();
  virtual ~kvs_driver();
  
  /*
   * 以下のメソッドは、サーバ層から呼び出される API である。
   */
  void put(const std::string& key, int32_t value);
  int32_t get(const std::string& key) const;
  float get_average() const;
  void get_status(std::map<std::string, std::string>& status) const;
  
  /*
   * 以下のメソッドは、`clear` / `save` / `load` RPC API が呼び出されたときに
   * Jubatus フレームワークによって実行される。
   * これらは定型句である。
   */
  void clear();
  void pack(jubatus::core::framework::packer& pk) const;
  void unpack(msgpack::object o);
  
private:
  // アルゴリズム層のインスタンス。
  jubatus::util::lang::shared_ptr<jubatus::core::kvs::kvs_algorithm> kvs_;
};
  
}  // namespace driver
}  // namespace core
}  // namespace jubatus

#endif  // KVS_DRIVER_HPP_
