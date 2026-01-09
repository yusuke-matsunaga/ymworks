#! /usr/bin/env python3

"""
パーサーの記述を生成するスクリプト

使い方:
  parse_gen -f parse_group.h -d Parser_dict.cc

データの定義:
  HandlerData のインスタンスに必要なデータを定義してゆく．
基本的にはグループ定義の集合となる．
グループを定義するには HandlerData.add_group_data()
を用いる．この関数は第１引数としてグループ名を，
第２引数としてヘッダの属性型をとる．
その他に以下に示すグループ本体の内容を示すオプション引数を取る．

 - simple_attrs:
   (属性名, 属性型) のタプルのリストを指定する．
   simple attribute で定義可能な属性型はあらかじめ決められている．

 - complex_attrs:
   (属性名, 属性型) のタプルのリストを指定する．
   complex attribute で定義可能な属性型はあらかじめ決められている．

 - groups:
   (属性名, グループ名) のタブルのリスト指定する．
   グループ名は他の場所で定義されている必要がある．

もしもそのグループが持たない属性タイプ(groupsなど)がある場合には
そのオプション引数は指定しない．

liberty では rise|fall や min|max など同様のパタンが組み合わされて属性名
となっていることが多い，そこで，属性名では (z|b|c) の記法を用いて
その部分を a, b, c に置き換えた３種類のパタンを生成するようにしている．
もちろｎ複数ヶ所にこの記法が現れた場合には直積が生成される．
liberty では識別子に '(' ')' の使用を許していないため，
この記法における '(' ')' のエスケープは不要である．

:file: parse_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2021 Yusuke Matsunaga, All rights reserved.
"""

import argparse
from handlerdata import HandlerData

parser = argparse.ArgumentParser()
parser.add_argument('-c', '--check', help='check integrity',
                    action='store_true')
parser.add_argument('-f', '--write-functions', help="write functions' definition to 'FILENAME'",
                    nargs=1, metavar='FILENAME')
parser.add_argument('-d', '--write-dict', help="write dict's entries to 'FILENAME'",
                    nargs=1, metavar='FILENAME')

args = parser.parse_args()
if not args:
    exit(1)

import library_data
import cell_data
import pin_data
import timing_data

hd = HandlerData()

hd.merge(library_data.hd)
hd.merge(cell_data.hd)
hd.merge(pin_data.hd)
hd.merge(timing_data.hd)


if args.check:
    if not hd.check():
        exit(1)

if args.write_functions:
    filename, = args.write_functions
    with open(filename, 'wt', encoding='utf_8_sig') as fout:
        hd.print_functions(fout=fout)

if args.write_dict:
    filename, = args.write_dict
    with open(filename, 'wt', encoding='utf_8_sig') as fout:
        hd.print_dict(fout=fout)
