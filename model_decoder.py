#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys
import codecs

"""
学習モデルのUnicodeエスケープをUTF-8に変換する
"""
print sys.argv
file=sys.argv[1]
decoded='decoded_'+file

print "{0} file open.".format(file)
with open(file, mode='r') as f:
    with open(decoded, mode='w') as wf:
        wf = codecs.lookup('utf-8')[-1](wf)
        for line in f:
#            print line.decode('unicode_escape')
            #encode("utf-8")しなくてもprintで表示はできる
            #リダイレクトでファイルに書き込む際にはencode('urf-8')が必要
            decoded_line = line.decode('unicode_escape').encode('utf-8')
            print decoded_line.rstrip()
#            wf.write(decoded_line)
wf.close()
f.close()
