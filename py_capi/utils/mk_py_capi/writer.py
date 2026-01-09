#! /usr/bin/env python3

""" Writer のクラス定義ファイル

:file: writer.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""


class Writer:
    """コードを出力するクラス
    字下げの管理のみ行う．
    """

    def __init__(self, *, fout):
        # 出力先のファイルオブジェクト
        self.__fout = fout
        # 現在のインデント値
        self.__indent = 0

    def gen_CRLF(self):
        """空行を出力する．
        """
        self.write_line('')

    def write_lines(self, lines, *,
                    delim=None):
        """複数行を出力する
        """
        n = len(lines)
        for i, line in enumerate(lines):
            if i < n - 1 and delim is not None:
                line += delim
            self.write_line(line)

    def write_line(self, line):
        """一行を出力する．
        """
        if line == '':
            self.__fout.write('\n')
        else:
            spc = ' ' * self.__indent
            self.__fout.write(f'{spc}{line}\n')

    def indent_inc(self, delta=2):
        """インデント量を増やす．
        """
        self.__indent += delta

    def indent_dec(self, delta=2):
        """インデント量を減らす
        """
        self.__indent -= delta

    def indent_set(self, val):
        """インデント量をセットする．
        """
        self.__indent = val
