#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <jubatus/client.hpp>

int main() {
    std::string host = "127.0.0.1";
    int port = 9199;
    std::string name = "test";
    int timeout = 10;

    // Classifierのクライアントをインスタンス化する
    jubatus::classifier::client::classifier client(host, port, name, timeout);

    // Datum の作成
    jubatus::client::common::datum d;
    d.add_string("name", "家康");

    // LabeledDatum(train メソッドで通知するデータ型) の作成
    jubatus::classifier::labeled_datum ld("徳川", d);

    // 学習用データの配列の作成
    std::vector<jubatus::classifier::labeled_datum> train_data;
    train_data.push_back(ld);

    // trainメソッドの呼び出し
    client.train(train_data);

    // 分析用データの配列の作成
    std::vector<jubatus::client::common::datum> test_data;
    test_data.push_back(d);

    // classifyメソッドの呼び出し
    std::vector<std::vector<jubatus::classifier::estimate_result> > results = client.classify(test_data);

    // 分析結果の出力
    for (size_t i = 0; i < results.size(); ++i) {
        for (size_t j = 0; j < results[i].size(); ++j) {
            const jubatus::classifier::estimate_result& r = results[i][j];
            std::cout << r.label << " " << r.score << std::endl;
        }
        std::cout << std::endl;
    }
}
