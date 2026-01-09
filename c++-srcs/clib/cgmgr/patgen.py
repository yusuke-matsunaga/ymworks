#! /usr/bin/env python3
# coding=UTF-8

# PatMgr.cc の分解パタンを作るスクリプト

import sys


def pg_sub(n, i0):
    """n 入力の分解パタンを列挙する．

    一つのパタンは整数のリスト
    この関数は i0 から始まる n 個のパタンのリストを返す．
    """

    if n == 1:
        return [[i0]]

    elif n == 2:
        return [[-1, i0, i0 + 1]]

    else:
        pat_list = []
        for nl in range(1, n):
            nr = n - nl
            pat_l_list = pg_sub(nl, i0)
            pat_r_list = pg_sub(nr, i0 + nl)

            for pat_l in pat_l_list:
                for pat_r in pat_r_list:
                    pat = [-1] + pat_l + pat_r
                    pat_list.append(pat)

        return pat_list


def patgen(ni):
    """ni 個の分解パタン用の配列データを出力する．"""
    if ni < 3:
        print("Error ni < 3")
        return

    pat_list = pg_sub(ni, 0)
    nl = len(pat_list)
    ne = ni * 2 - 1

    print("// {}入力の分解パタン".format(ni))
    print("const int n_pat{0} = {1};".format(ni, nl))
    print("int pat{0}[n_pat{0}][{1}] = {{".format(ni, ne))
    for p in range(0, nl):
        pat = pat_list[p]
        line = "\t{"
        comma = ""
        for i in range(0, ne):
            line += comma
            line += " {}".format(pat[i])
            comma = ","
        line += " }"
        if p < nl - 1:
            line += ","
        print(line)
    print("};")
    print()


# main 関数
#
# USAGE: patgen.py <最大入力数>

if len(sys.argv) != 2:
    print("Usage: {} <最大入力数>".format(sys.argv[0]))
    sys.exit(1)

maxni = int(sys.argv[1]) + 1
for ni in range(3, maxni):
    patgen(ni)
