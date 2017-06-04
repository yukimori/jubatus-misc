import jubatus
import os
from jubatus.common import Datum

"""
以下の設定ファイルに対してテスト可能
mecab.json
mecab_advanced.json
tf-idf.json
tf-bm25.json
"""

client = jubatus.Weight("127.0.0.1", 9199, "")

# データ必要なidf,bm25では値が出力されない
with open("./dataset/text/sports-watch/sports-watch-5252498.txt") as f:
    lines = f.readlines()
    title = lines[2].strip()
    d = Datum({"title": title})
    print("d {}".format(d))
    print(client.calc_weight(d))


# tf-idfを使うためにデータを登録する
files = os.listdir("./dataset/text/sports-watch")
datum_dict = {}
for fname in files:
    # print("fline : {}".format(fname))
    with open("./dataset/text/sports-watch/{}".format(fname)) as f:
        lines = f.readlines()
        # if fname in "sports-watch-5252498.txt":
        #     print("lines {}".format(lines))
        title = lines[2].strip()
        d = Datum({"title": title})
        client.update(d)
        datum_dict[fname] = d

data = datum_dict["sports-watch-5252498.txt"]
print("tf-idf data {}".format(data))
print(client.calc_weight(data))
