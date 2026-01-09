#! /usr/bin/env python3

""" Partial-DAG を列挙するプログラム

:file: pdgen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import itertools


class Fence:
    """ fence の情報を表すクラス

    基本的な情報は各レベルのノード数だが，Partial DAG を列挙
    するために各レベルの先頭のノード番号も保持する．
    """

    def __init__(self, n_list):
        self.__num_list = n_list[:]
        self.__level = len(n_list)
        self.__top_list = []
        self.__level_list = []
        cur = 0
        for level, n in enumerate(n_list):
            self.__top_list.append(cur)
            cur += n
            for _ in range(n):
                self.__level_list.append(level)
        # end-marker
        self.__top_list.append(cur)

    @property
    def max_level(self):
        """最大レベル"""
        return self.__level

    @property
    def total_num(self):
        """全ノード数"""
        return self.__top_list[-1]

    def num(self, level):
        """level のノード数を返す．
        """
        if level >= self.__level:
            raise ValueError("level is out of range")
        return self.__num_list[level]

    def top(self, level):
        """level のノードの先頭番号を返す．
        """
        if level >= self.__level:
            raise ValueError("level is out of range")
        return self.__top_list[level]

    def level(self, node_id):
        """node_id のレベルを返す．
        """
        if node_id >= self.total_num:
            raise ValueError("node_id is out of range")
        return self.__level_list[node_id]

    def display(self):
        print('(', end='')
        for n in self.__num_list:
            print(f'{n}', end='')
        print(')')


def generate_fence(k, l, *, operand_num=2):
    """ノード数k，最大レベルlの fence を列挙する．
    :param int k: ノード数
    :param int l: 最大レベル
    :param int operand_num: 演算の項数
    """
    assert l > 0
    assert k >= l

    def check_feasibility(n_list):
        """実現可能な fence かチェックする．
        """
        l = len(n_list)
        limit = 1
        for i in range(l - 1, -1, -1):
            if n_list[i] > limit:
                return False
            limit *= operand_num
        return True

    def fence_sub(cur_n_list, l, pos, r, fence_list):
        if pos == l:
            # 終端
            fence_list.append(Fence(cur_n_list))
        else:
            if pos < l - 1:
                start = 0
            else:
                start = r
            for i in range(start, r + 1):
                tmp_n_list = cur_n_list[:] # コピー
                tmp_n_list[pos] += i
                if check_feasibility(tmp_n_list):
                    fence_sub(tmp_n_list, l, pos + 1, r - i, fence_list)

    # 各レベル１つのノードは必ず割り当てられるので
    # 残りのノードの配置問題となる．
    r = k - l
    n_list = [ 1 for _ in range(l) ]
    fence_list = []
    fence_sub(n_list, l, 0, r, fence_list)
    return fence_list


def check_ord(pd):
    """各演算のオペランドの順序関係のチェック
    """

    def check_le(op1, op2):
        """オペランド番号の比較
        実際にはオペランド番号のタプルなので
        辞書式順序で比較する．
        """
        n1 = len(op1)
        n2 = len(op2)
        n = min(n1, n2)
        for i in range(n):
            if op1[i] < op2[i]:
                return True
            if op1[i] > op2[i]:
                return False
            # op1[i] == op2[i]
        # op1[:n] == op2[:n]
        if n1 > n:
            return False
        return True

    l = len(pd)
    assert l > 0
    for i in range(l - 1):
        op1 = pd[i]
        op2 = pd[i + 1]
        if not check_le(op1, op2):
            return False
    return True


def check_noreapplication(pd):
    # 自分のオペランドと共に他のノードのオペランドとして
    # 用いられていないことをチェックする．
    # op2 が2項演算の場合のみ
    l = len(pd)
    assert l > 0
    for i in range(l - 1):
        op1 = pd[i]
        for j in range(i + 1, l):
            op2 = pd[j]
            if len(op2) > 2:
                continue
            y1, y2 = op2
            if y2 != i:
                continue
            for x in op1:
                if x != -1 and x == y1:
                    return False
    return True


def check_all_use(pd):
    # 全てのノードがオペランドとして用いられているか調べる．
    l = len(pd)
    assert l > 0
    marks = [ False for _ in range(l) ]
    for op in pd:
        for x in op:
            if x >= 0:
                marks[x] = True
    # 最後のノードは出力として用いられる．
    marks[-1] = True
    for mark in marks:
        if not mark:
            return False
    return True


def check_pd(pd):
    """正しい Partial DAG か調べる．
    具体的には以下の項目を調べる．
    - 前後のオペランド番号対の間に順序関係が成り立っているか．
    - op_i = (x1, x2) の時，(x1, i) もしくは (x2, i) というオペランドがない．
    - 全てのノードがいずれかのオペランドとして用いられている．
    """
    if not check_ord(pd):
        return False
    if not check_noreapplication(pd):
        return False
    if not check_all_use(pd):
        return False
    return True


def generate_pd(fence, *, operand_num=2):
    """fence の制約を満たす Partial-DAGを列挙する．
    結果は ((-1, -1), (-1, -1), (-1, 0), (-1, 1)) の様な
    整数のペアのタプルとなる．
    それぞれのノードのオペランドの番号を表す．
    ノード番号は 0 から始まる．
    -1 は外部入力(のどれか)を表す．

    レベルlのノードのオペランドはレベル l - 1のノードを
    一つ以上含めなければならない．
    さらにオペランド間の順序制約があるので最後のオペランド
    は必ずレベルl - 1のノードとなる．

    同じレベルのノード同士にもオペランドの番号による順序関係が
    ある．具体的には前のノードのオペランドを(x1, x2)，後のノード
    のオペランドを(y1, y2)とした時に
    (x2 < y2) or ((x2 == y2) and (x1 <= y1))
    の関係が成り立たなければならない．
    """

    def pd_sub(cur_pd, fence, node_id, pd_list):
        if node_id == fence.total_num:
            if check_pd(cur_pd):
                pd_list.append(tuple(cur_pd))
            return
        l = fence.level(node_id)
        if l == 0:
            # レベル0のノードのオペランドはすべて -1
            for ni in range(2, operand_num + 1):
                tmp_pd = cur_pd[:] # コピー
                tmp_pd.append([ -1 for _ in range(ni) ])
                pd_sub(tmp_pd, fence, node_id + 1, pd_list)
            return
        top = fence.top(l)
        top0 = fence.top(l - 1)
        # オペランドを列挙する．
        # 候補は -1 か 0〜top-1 で，-1 は複数回選ばれてもよい．
        # 最後の要素は top0 以上である必要がある．
        # -1 のみ特別なので別に処理する．
        src_list = [ x for x in range(top) ]
        for ni in range(2, operand_num + 1):
            for c in range(0, ni):
                opr_list0 = [ -1 for _ in range(c) ]
                for opr_list1 in itertools.combinations(src_list, ni - c):
                    opr_list = opr_list0[:]
                    for x in opr_list1:
                        opr_list.append(x)
                    if opr_list[-1] < top0:
                        continue
                    tmp_pd = cur_pd[:] # コピー
                    tmp_pd.append(tuple(opr_list))
                    pd_sub(tmp_pd, fence, node_id + 1, pd_list)

    l = fence.max_level
    assert l > 0
    pd_list = []
    pd_sub([], fence, 0, pd_list)
    return pd_list


if __name__ == '__main__':

    for k in range(2, 10):
        for l in range(1, k + 1):
            i = 2
            fence_list = generate_fence(k, l, operand_num=i)
            c = 0
            if len(fence_list) > 0:
                for fence in fence_list:
                    pd_list = generate_pd(fence, operand_num=i)
                    c += len(pd_list)
            if c > 0:
                print(f'{k} & {l} & {c} \\')
