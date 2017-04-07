https://github.com/jubatus/jubatus_core/pull/356
の確認．

同方向特徴ベクトルのコサイン類似度が1.0を超えるというものFloatの誤差が入って.

(1.0, 1.0, 0, 0)
(0.0001, 0.0001, 0, 0)

で発生する．大きさを少し変えると発生しないことがある．

https://gist.github.com/TkrUdagawa/44d93ccf3d605ccea14eec50ba3ddc1c
で宇田川君がjubatus_coreを直接呼び出すコードを書いている

テストコードであるinverted_index_storage_test.cppを書き換えて確認．
コードだけこちらにコピーする．