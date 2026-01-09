#! /usr/bin/env python3

""" Partial-DAG を列挙するプログラム

:file: pdgen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import itertools
import re

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
                tmp_pd.append(tuple( -1 for _ in range(ni) ))
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

def check_bd(bd, input_num):
    """正しい Bound DAG か調べる．
    具体的には以下の項目を調べる．
    - 前後のオペランド番号対の間に順序関係が成り立っているか．
    - op_i = (x1, x2) の時，(x1, i) もしくは (x2, i) というオペランドがない．
    - 全ての入力が使われているか
    """

    def check_opr(opr0, opr1):
        ni0 = 0
        for op in opr0:
            if op < input_num:
                ni0 += 1
        ni1 = 0
        for op in opr1:
            if op < input_num:
                ni1 += 1
        if ni0 > ni1:
            return True
        if ni0 < ni1:
            return False
        n0 = len(opr0)
        n1 = len(opr1)
        if n0 != n1:
            return True
        for i in range(n0):
            op0 = opr0[i]
            op1 = opr1[i]
            if op0 > op1:
                return False
            if op0 < op1:
                return True
            # op0 == op1:
        return True

    l = len(bd)
    for i in range(l - 1):
        opr0 = bd[i]
        opr1 = bd[i + 1]
        if not check_opr(opr0, opr1):
            #print('check_opr(opr0, opr1) => False')
            return False
    for i in range(l - 1):
        opr0 = bd[i]
        if len(opr0) != 2:
            continue
        op0, op1 = opr0
        for j in range(i + 1, l):
            opr1 = bd[j]
            if len(opr1) != 2:
                continue
            op3, op4 = opr1
            if op4 != i + input_num:
                continue
            if op3 == op0 or op3 == op1:
                #print('op3 == op0 or op3 == op1 => False')
                return False
    mark = [ False for _ in range(input_num) ]
    for opr in bd:
        for op in opr:
            if op < input_num:
                mark[op] = True
    for i, m in enumerate(mark):
        if not m:
            #print(f'mark[{i}] => False')
            return False
    return True


def generate_bd(pd, input_num):
    """Partial-DAG から Bound-DAG を列挙する．
    結果は (('i0', 'i1'), ('i2', 'i3'), ('i4', 'n0'), ('i3', 'n1'))
    の様な文字列のペアのタプルとなる．
    'ix' は x 番目の入力を表す．
    'nx' は x 番目の演算ノードを表す．
    入力番号とノード番号は 0 から始まる．

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

    def bd_sub(cur_bd, pd, node_id, bd_list):
        if node_id == len(pd):
            if check_bd(cur_bd, input_num):
                bd_list.append(tuple(cur_bd))
            return
        opr_list = pd[node_id]
        pos_list = [ i for i, opr in enumerate(opr_list) if opr == -1 ]
        r = len(pos_list)
        if r == 0:
            new_bd = cur_bd[:]
            new_bd.append([ opr + input_num for opr in opr_list ])
            bd_sub(new_bd, pd, node_id + 1, bd_list)
        else:
            for i_list in itertools.combinations(range(input_num), r):
                bound_list = []
                x = 0
                for opr in opr_list:
                    if opr == -1:
                        i = i_list[x]
                        x += 1
                        bound_list.append(i)
                    else:
                        bound_list.append(opr + input_num)
                new_bd = cur_bd[:]
                new_bd.append(bound_list)
                bd_sub(new_bd, pd, node_id + 1, bd_list)

    bd_list = []
    bd_sub([], pd, 0, bd_list)
    return bd_list


class SbdNode:
    """複数の Bound DAG を共有して表現するためのノード
    もとの Partial DAG の一つのノードから派生した
    複数の Bound DAG のノードが存在する．
    それらは真理値表の変数を共有できるのでもとの
    Partial DAG のノード番号も持つ．
    """

    def __init__(self, op_id, pd_id, operands):
        self.__op_id = op_id
        self.__pd_id = pd_id
        self.__operands = tuple(operands)

    @property
    def op_id(self):
        return self.__op_id

    @property
    def pd_id(self):
        return self.__pd_id

    @property
    def operands(self):
        return self.__operands


class SbdGraph:
    """複数の Bound Graph を共有して表現するためのグラフ
    """

    def __init__(self, node_list, root_list):
        self.__node_list = tuple(node_list)
        self.__root_list = tuple(root_list)

    @property
    def root_list(self):
        return self.__root_list

    @property
    def node_num(self):
        return len(self.__node_list)

    def node(self, node_id):
        return self.__node_list[node_id]

    def print(self):
        pass


def make_sbd(bd_list, input_num):
    """複数の Bound DAG を共有したグラフを作る．
    bd_list 中の Bound DAG は入力割当以外はすべて
    等しい構造を持つと仮定する．
    """
    node_map = {}
    node_list = []
    root_list = []
    for bd in bd_list:
        local_map = {}
        for x in range(input_num):
            local_map[x] = x
        for op_id, opr in enumerate(bd):
            sbd_operands = []
            for x in opr:
                op_id1 = local_map[x]
                sbd_operands.append(op_id1)
            # 同じノードが存在しているか調べる．
            key = (op_id, tuple(sbd_operands))
            if key in node_map:
                node_id = node_map[key]
            else:
                node_id = len(node_list)
                node = SbdNode(node_id, op_id, sbd_operands)
                node_list.append(node)
                node_map[key] = node_id
            local_map[op_id + input_num] = node_id
        root_list.append(local_map[len(bd) + input_num - 1])
    return SbdGraph(node_list, root_list)


if __name__ == '__main__':

    from argparse import ArgumentParser

    parser = ArgumentParser(prog='pdggen',
                            description='Partial DAG generater')
    parser.add_argument('k', type=int,
                        help='# of nodes')
    parser.add_argument('l', type=int,
                        help='max level')
    parser.add_argument('ni', type=int,
                        help='number of inptus')
    parser.add_argument('-o', '--operand_num',
                        type=int, default=2,
                        help='# of operands')

    args = parser.parse_args()
    if args is None:
        exit(1)

    k = args.k
    l = args.l
    ni = args.ni
    i = args.operand_num
    fence_list = generate_fence(k, l, operand_num=i)
    if len(fence_list) > 0:
        print(f'k = {k}, l = {l}')
        for fence in fence_list:
            print('fence: ', end='')
            fence.display()
            pd_list = generate_pd(fence, operand_num=i)
            for pd in pd_list:
                print('PD: ', end='')
                print(pd)
                bd_list = generate_bd(pd, ni)
                for bd in bd_list:
                    print(bd)
                print(f'# of BDs = {len(bd_list)}')
                sbd = make_sbd(bd_list, ni)
                print(f'# of SBD nodes = {sbd.node_num}')
