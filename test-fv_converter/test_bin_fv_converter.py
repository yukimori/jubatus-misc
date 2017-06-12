#-*- coding:utf-8 -*-

import jubatus
from jubatus.common import Datum

"""
画像特徴抽出プラグインを試す
"""

# ファイルをバイナリモードで開く
with open("./dataset/Lenna.jpg", "br") as f:
    data = f.read()
    d = Datum()
    d.add_binary("image", data)


client = jubatus.Weight("127.0.0.1", 9199, "")
res = client.calc_weight(d)
print(res)

