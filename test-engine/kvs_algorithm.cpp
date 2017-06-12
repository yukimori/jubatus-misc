#include "kvs_algorithm.hpp"

#include <string>

#include <jubatus/util/lang/cast.h>

using jubatus::util::lang::lexical_cast;

namespace jubatus {
  namespace core {
    namespace kvs {

      /**
       * Update: `put` 操作は、学習モデルに指定された key-value の組を追加する。
       * key が既に存在する場合は上書きされる。
       */
      void kvs_algorithm::put(const std::string& key, int32_t value) {
	/*
	 * key が学習モデル内に既に存在するかどうかを確認する。
	 * もし key が存在すれば、後で `local_sum_` を更新するために古い値を保持しておく。
	 */
	int32_t old_value = 0;
	if (local_data_.find(key) != local_data_.end()) {
	  old_value = local_data_[key];
	}

	/*
	 * 学習モデルを更新する。
	 */
	local_data_[key] = value;
	local_sum_ = local_sum_ - old_value + value;
      }

      /**
       * Analyze: `get` 操作は、学習モデル内の指定された key に対応する value を返却する。
       * もし key がモデルに存在しなければ例外を送出する。
       */
      int32_t kvs_algorithm::get(const std::string& key) const {
	data_t::const_iterator it = local_data_.find(key);
	if (it != local_data_.end()) {
	  return it->second;
	}
	throw std::out_of_range(key + " not found");
      }

      /**
       * Analyze: `get_average` 操作は、クラスタ全体における value の平均値を返却する。
       */
      float kvs_algorithm::get_average() const {
	return (float(global_sum_ + local_sum_) / (global_size_ + local_data_.size()));
      }

      /**
       * MIX: MIX セッションが開始されると、MIX マスタは全サーバに対して `get_diff` RPC
       * API を呼び出し、各サーバの学習モデルの差分情報　(`diff` オブジェクト) を収集する。
       * このメソッドは、クラスタ全体で共有される必要がある、このサーバの学習モデルの差分情報
       * を返却する必要がある。
       *
       * 本 KVS では、このサーバ内のすべての value の合計と、key の数を `diff` オブジェクトとして
       * 返却する (クラスタ全体の value の平均値を求めることができればよいので、各サーバの
       * Key-Valueデータ全体 (`local_data_`) をクラスタ全体で共有する必要はない)。
       */
      void kvs_algorithm::get_diff(kvs_diff& diff) const {
	diff.sum_ = local_sum_;
	diff.size_ = local_data_.size();
      }

      /**
       * MIX: `mix` は二つの `diff` オブジェクトを結合する静的な操作である。
       * MIX マスタサーバは、クラスタ内から収集したすべての `diff` オブジェクトを、この関数で
       * 1 つの `diff` オブジェクトに畳み込む。
       *
       * 本 KVS では、クラスタ内から集めた `diff` の各値を合計することで、クラスタ内の全 value
       * の合計とクラスタ内の全 key 数を求める。
       */
      void kvs_algorithm::mix(const kvs_diff& lhs, kvs_diff& rhs) const {
	rhs.sum_ += lhs.sum_;
	rhs.size_ += lhs.size_;
      }

      /**
       * MIX: MIX セッションの終わりに、MIX マスタは全サーバに対して `put_diff` を呼び出す。
       * このメソッドでは、MIX マスタから送信された `mixed_diff` (クラスタ内のすべての `diff`
       * オブジェクトを `mix` メソッドによって 1 つに畳み込んだもの) を、自身の学習モデルに反映
       * する処理を行う。
       *
       * 本 KVS では、MIX 時点でのクラスタ全体(自分自身を除く)の value の合計 / キー数の合計を
       * `global_sum_` / `global_size_` に格納することで、`get_average` がクラスタ全体の value
       * の平均値を返却できるようにする。
       */
      bool kvs_algorithm::put_diff(const kvs_diff& mixed_diff) {
	global_sum_ = mixed_diff.sum_ - local_sum_;
	global_size_ = mixed_diff.size_ - local_data_.size();

	/*
	 * `put_diff` が成功したことを意味する `true` を返却する。
	 */
	return true;
      }

      /**
       * MIX: `get_version` は、学習モデルのシーケンシャルなリビジョン番号を返却する。
       * この仕組みは、クラスタ内の全サーバが同じリビジョン番号の学習モデルを持つことを保証するために使用できる。
       * このような保証が必要な場合は、上記の MIX コードにリビジョンの検証処理を実装する必要がある。
       * 実装例は、Jubatus フレームワークの `jubatus::core::storage::local_storage_mixture`
 * クラスを参照のこと。
 *
       * 本 KVS では、MIX が冪等な操作である (つまり、一部のサーバが MIX セッションに参加できなかったとしても、
       * 次の MIX セッションに参加すればクラスタ内の学習モデルはすべて統一される) ことから、リビジョンの検証は
 * 不要であるため、常に同一のバージョン値を返却する。
 */
      jubatus::core::storage::version kvs_algorithm::get_version() const {
	return jubatus::core::storage::version();
      }

      /**
       * KVS 固有のメトリクス値を返却する。
       */
      void kvs_algorithm::get_status(std::map<std::string, std::string>& status) const {
	status.insert(std::make_pair(
				     "local_size", lexical_cast<std::string>(local_data_.size())));
	status.insert(std::make_pair(
				     "global_size", lexical_cast<std::string>(global_size_)));
      }

      /**
       * 学習モデルを初期状態に戻す。
       */
      void kvs_algorithm::clear() {
	local_data_ = data_t();
	local_sum_ = 0;
	global_sum_ = 0;
	global_size_ = 0;
      }

      /**
       * 学習モデルを MessagePack 形式でシリアライズする。
       */
      void kvs_algorithm::pack(jubatus::core::framework::packer& pk) const {
	pk.pack(*this);
      }

      /**
       * MessagePack 形式の学習モデルをデシリアライズする。
       */
      void kvs_algorithm::unpack(msgpack::object o) {
	o.convert(this);
      }

    }  // namespace kvs
  }  // namespace core
}  // namespace jubatus
