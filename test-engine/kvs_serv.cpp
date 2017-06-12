// This file is auto-generated from kvs.idl with jenerator version /master

#include "kvs_serv.hpp"

#include <utility>

// Mixer インスタンスを作成するために必要なヘッダファイル
#include <jubatus/server/framework/mixer/mixer_factory.hpp>

// フレームワークの提供するロガーを使用するために必要なヘッダファイル
#include <jubatus/server/common/logger/logger.hpp>

namespace jubatus {
namespace server {

namespace {
  /**
   * 設定ファイル(JSON)のフォーマットを定義する構造体。
   *
   * 本 KVS には設定可能なパラメタは存在しないため、これらの値は実際の処理には
   * 使用されないが、設定ファイルの取り扱い方法を例示するためにダミーのパラメタを用意している。
   * 詳細は以下の `set_config` メソッドをのコメントを参照。
   */
  struct kvs_serv_config {
    int param1;
    std::string param2;
    
    /*
     * `serialize` メソッドは JSON 形式と `struct` の相互変換を容易に行う
     * ために必要となる。
     */
    template <typename Ar>
    void serialize(Ar& ar) {
      ar
	& JUBA_MEMBER(param1)
	& JUBA_MEMBER(param2);
    }
  };
}


/**
 * コンストラクタ。エンジンの初期化処理を行う。
 * ドライバ層および Mixer の生成は定型句である。
 * 必要に応じてエンジン固有の初期化処理を行うことができる。
 *
 * この時点では、サーバ設定ファイル(JSON)の内容は利用できない点に注意する。
 */  
kvs_serv::kvs_serv(
  const jubatus::server::framework::server_argv& a,
  const jubatus::util::lang::shared_ptr<jubatus::server::common::lock_service>& zk)
  : jubatus::server::framework::server_base(a),
    // ドライバ層のインスタンスを生成
    driver_(new jubatus::core::driver::kvs_driver()),
    // Mixer(Mixを行うスレッド)を生成
    mixer_(jubatus::server::framework::mixer::create_mixer(
							   a, zk, rw_mutex(), user_data_version())) {
  // Mixerに対してdriver層のインスタンスを設定することで当該ドライバをMixの対象とする
  mixer_->set_driver(driver_.get());
}

kvs_serv::~kvs_serv() {
}

jubatus::server::framework::mixer::mixer* kvs_serv::get_mixer() const {
  return mixer_.get();
}

jubatus::core::driver::driver_base* kvs_serv::get_driver() const {
  return driver_.get();
}

std::string kvs_serv::get_config() const {
  return config_;
}

uint64_t kvs_serv::user_data_version() const {
  return 1u;
}

void kvs_serv::get_status(status_t& status) const {
  driver_ -> get_status(status);
}

  /**
   * このメソッドは、設定ファイルの読み込みが成功した時点で 1 度だけ実行される。
   *
   * すべての RPC API 呼び出しは、 `set_config` の処理が完了した後に
   * 行われると仮定してよい。
   */
void kvs_serv::set_config(const std::string& config) {
  /*
   * Jubatus フレームワークは、JSON と構造体のマッピングを行う `jsonconfig`
   * モジュールを提供している。 `jsonconfig` を使用して、JSON 文字列 (`config`)
   * を `struct kvs_serv_config` 型に変換する。変換先の構造体は `serialize`
   * メソッドを定義している必要がある (上の定義を参照)。
   */
  core::common::jsonconfig::config config_root(
					       jubatus::util::lang::lexical_cast<jubatus::util::text::json::json>(config));
  kvs_serv_config conf =
    core::common::jsonconfig::config_cast_check<kvs_serv_config>(config_root);

  /*
   * `set_config` API が参照するため、設定ファイルの内容をフィールドに保存しておく必要がある。
   */
  config_ = config;

  /*
   * 設定値の参照方法を以下に示す。機械学習アルゴリズムを実装する場合、この `set_config` の
   * 仕組みはハイパーパラメタを引き渡すために使用することができる。
   *
   * この例(KVS)では、パラメタは実際には使用されない。
   * 以下のコードは、設定値の参照方法と、ログ出力のサンプルを示している。
   */
  LOG(INFO) << "param1 = " << conf.param1;
  LOG(INFO) << "param2 = " << conf.param2;
}

bool kvs_serv::put(const std::string& key, int32_t value) {
  /*
   * ドライバ層に処理を委譲する。
   */
  driver_->put(key, value);
  return true;
}

entry kvs_serv::get(const std::string& key) const {
  /*
   * ドライバ層に処理を委譲する。
   */
  int32_t v = driver_->get(key);

  /*
   * ドライバ層からの戻り値を RPC インタフェースで使用するデータ構造に格納する。
   */
  entry result;
  result.key = key;
  result.value = v;

  /*
   * 結果を返却する。
   */
  return result;
}

float kvs_serv::get_average() const {
  /*
   * ドライバ層に処理を委譲する。
   */
  return driver_->get_average();
}

}  // namespace server
}  // namespace jubatus
