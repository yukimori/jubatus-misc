import pandas as pd

# "pref" 列をインデックスに、csv を読み込む
f = pd.read_csv("./dataset/number.csv", index_col="pref")
# 北海道のデータを表示する
print(f.loc("Hokkaido"))
print(f.loc["Hokkaido"])
