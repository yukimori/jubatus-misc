#include <iostream>

// クライアントライブラリのヘッダファイルをインクルードする
#include <kvs_client.hpp>

int main() {
  // クライアントインスタンスを生成する
  jubatus::client::kvs client("127.0.0.1", 9199, "test", 0);

  // put RPC を送信する
  std::cout << "put(key1, 123) = " << client.put("key1", 123) << std::endl;
  std::cout << "put(key2, 456) = " << client.put("key2", 456) << std::endl;

  // get RPC を送信する
  std::cout << "get(key1) = " << client.get("key1").value << std::endl;
  std::cout << "get(key2) = " << client.get("key2").value << std::endl;

  // get_average RPC を送信する
  std::cout << "get_average() = " << client.get_average() << std::endl;

  return 0;
}
