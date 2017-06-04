import pandas as pd
from jubatus.common import Datum

"""
数値特徴変換を試す
"""

def make_datum(data, headers):
    '''
    ヘッダのリストとデータの行を1 行受け取り、
    datum を作成する関数
    '''
    d = Datum()
    for k in headers:
        d.add_number(k, data[k])
    return d


# "pref" 列をインデックスに、csv を読み込む
f = pd.read_csv("./dataset/number.csv", index_col="pref")
# 北海道のデータを表示する
print(f.loc["Hokkaido"])

# 最大値を取得する
print(f.apply(lambda x: x.max()))

# 最小値を取得する
print(f.apply(lambda x: x.min()))

headers = list(f.columns)
d_hokkaido = make_datum(f.loc["Hokkaido"], headers)
print(d_hokkaido)

import jubatus

client = jubatus.Weight("127.0.0.1", 9199, "")
res = client.calc_weight(d_hokkaido)
print(res)

# print(res[0])
# print(type(res[0]))
# print(res[0].key)

d_tokyo = make_datum(f.loc["Tokyo-to"], headers)
print(client.update(d_tokyo))
