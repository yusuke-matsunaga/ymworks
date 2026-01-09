#! /usr/bin/env python3

""" 5入力のNPN代表関数を列挙するスクリプト

:file: gen_rep5.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import itertools


class Npn5:
    """5入力関数のNPN変換を表すクラス

    具体的には
    * 入力の反転
    * 入力順
    * 出力の反転
    の情報を持つ．
    """
    def __init__(self, *,
                 iinv_list=[False, False, False, False, False],
                 iperm=[0, 1, 2, 3, 4],
                 oinv=False):
        self.__iinv_list = iinv_list
        self.__iperm = iperm
        self.__oinv = oinv

    def iinv(self, pos):
        """入力の反転属性を返す．
        :param int pos: 入力番号 ( 0 <= pos < 5 )
        """
        if pos < 0 or 5 <= pos:
            raise ValueError
        return self.__iinv_list[pos]

    def iperm(self, pos):
        """入力の順番を返す．
        :param int pos: 入力番号 ( 0 <= pos < 5 )
        """
        if pos < 0 or 5 <= pos:
            raise ValueError
        return self.__iperm[pos]

    def oinv(self):
        """出力の反転属性を返す．
        """
        return self.__oinv

    def invert(self):
        """逆変換を返す．
        """
        oinv = self.oinv()
        iinv_list = [False, False, False, False, False]
        iperm = [0, 0, 0, 0, 0]
        for i in range(5):
            j = self.iperm(i)
            iinv_list[j] = self.iinv(i)
            iperm[j] = i
        return Npn5(iinv_list=iinv_list, iperm=iperm, oinv=oinv)

    def xform(self, tv):
        """関数にNPN変換を施す．
        :param int tv: 関数を表すビットベクタ(32ビットの符号なし整数)
        """
        if self.oinv():
            tv = ~tv & 0xFFFFFFFF
        if self.iinv(0):
            tv0 = tv & 0x55555555
            tv1 = tv & 0xAAAAAAAA
            tv = (tv1 >> 1) | (tv0 << 1)
        if self.iinv(1):
            tv0 = tv & 0x33333333
            tv1 = tv & 0xCCCCCCCC
            tv = (tv1 >> 2) | (tv0 << 2)
        if self.iinv(2):
            tv0 = tv & 0x0F0F0F0F
            tv1 = tv & 0xF0F0F0F0
            tv = (tv1 >> 4) | (tv0 << 4)
        if self.iinv(3):
            tv0 = tv & 0x00FF00FF
            tv1 = tv & 0xFF00FF00
            tv = (tv1 >> 8) | (tv0 << 8)
        if self.iinv(4):
            tv0 = tv & 0x0000FFFF
            tv1 = tv & 0xFFFF0000
            tv = (tv1 >> 16) | (tv0 << 16)
        xtv = 0
        for b in range(32):
            if tv & (1 << b):
                new_b = 0
                for i in range(5):
                    if b & (1 << i):
                        new_b |= (1 << self.iperm(i))
                xtv |= (1 << new_b)
        return xtv

def check_rep(tv):
    for b in range(64):
        oinv = ((b >> 5) & 1) == 1
        iinv_list = [ ((b >> i) & 1) == 1 for i in range(5) ]
        for perm in perm_list:
            xf = Npn5(iinv_list=iinv_list, iperm=perm, oinv=oinv)
            tv1 = xf.xform(tv)
            if tv1 < tv:
                return False
    return True


if __name__ == '__main__':

    func5_size = 0x100000000
    perm_list = [ order_list
                  for order_list in itertools.permutations([0, 1, 2, 3, 4], 5)]
    rep_count = 0
    for tv in range(func5_size):
        if check_rep(tv):
            rep_count += 1
    print(f'Total {rep_count} rep. funcs')
