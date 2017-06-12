#ifndef KVS_ALGORITHM_HPP_
#define KVS_ALGORITHM_HPP_

#include <stdint.h>
#include <utility>
#include <string>

#include <msgpack.hpp>

#include <jubatus/core/framework/mixable_helper.hpp>

namespace jubatus {
  namespace core {
    namespace kvs {

      /**
       * `kvs_diff` 構造体は、KVS の `diff` オブジェクトである。
       * 詳細は `get_diff` のコメントを参照のこと。
       *
       * `diff` オブジェクトは、MessagePack でシリアライズ可能である必要があるため、
       * `MSGPACK_DEFINE` マクロにフィールドが列挙されている。
       */
      struct kvs_diff {
	int32_t sum_;
	uint32_t size_;

	MSGPACK_DEFINE(sum_, size_);
      };

      /**
       * `kvs_algorithm` クラスは、実際の Update/Analyze/MIX 処理を実装するクラスである。
       * 詳細は、各メソッドの実装のコメントを参照のこと。
       *
       * また、このクラスは学習モデルをフィールドとして保持する (以下を参照のこと)。
       */
      class kvs_algorithm {
      public:
	kvs_algorithm()
	  : local_sum_(0),
	    global_sum_(0),
	    global_size_(0) {};
	virtual ~kvs_algorithm() {};

	// API:
	void put(const std::string& key, int32_t value);
	int32_t get(const std::string& key) const;
	float get_average() const;

	// MIX 用の API:
	// (これら 4 つのメソッドは、以下の `linear_mixable_helper` を使用するために必要となる)
	void get_diff(kvs_diff& diff) const;
	void mix(const kvs_diff& lhs, kvs_diff& rhs) const;
	bool put_diff(const kvs_diff& mixed_diff);
	jubatus::core::storage::version get_version() const;

	// `clear` API:
	void clear();

	// `get_stauts` API:
	void get_status(std::map<std::string, std::string>& status) const;

	// `save` / `load` API:
	void pack(jubatus::core::framework::packer& pk) const;
	void unpack(msgpack::object o);

      private:
	typedef std::map<std::string, int32_t> data_t;

	/**
	 * `local_data_` は `put` によって追加された key-value データを保持する。
	 */
	data_t local_data_;

	/**
	 * `local_sum_` は `local_data_` 内の value の合計値を保持する。
	 * この値は `put` メソッド内で更新される。
	 */
	int32_t local_sum_;

	/**
	 * `global_sum_` および `global_size_` は、クラスタ全体(ただし自分自身を除く)の
	 * value / Key 数の合計値である。これらの値は MIX 時に更新される。
	 */
	int32_t global_sum_;
	uint32_t global_size_;

      public:
	MSGPACK_DEFINE(local_data_, local_sum_, global_sum_, global_size_);
      };

      /**
       * 以下のコードは、Linear MIX に対応した `kvs_algorithm` クラス (`mixable_kvs` クラス)
       * を定義している。
       *
       * `linear_mixable_helper` は Jubatus フレームワークの提供するヘルパクラスで、
       * `get_diff`, `mix`, `put_diff` の操作を簡単に書くことを可能にする。このヘルパクラスを
       * 使用しない場合は、生の MIX 処理を記述した別の `mixable` クラス (`linear_mixable`
       * クラスを継承) を自身で定義する必要がある。
       *
       * `linear_mixable_helper` は Linear MIX だけに対応する点に注意すること。つまり、
       * Push/Pull MIX による学習モデルの MIX を行いたい場合は、別の `mixable` クラス
       * (`push_mixable` クラスを継承) を自身で定義する必要がある。なお、1 つの `mixable`
       * クラスは `lienar_mixable` と `push_mixable` を同時に継承することもできる。
       */
      typedef jubatus::core::framework::linear_mixable_helper<kvs_algorithm, kvs_diff>
      mixable_kvs;

    }  // namespace kvs
  }  // namespace core
}  // namespace jubatus

#endif  // KVS_ALGORITHM_HPP_
