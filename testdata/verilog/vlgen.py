#! /usr/bin/env python3

"""

:file: vlgen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2026 Yusuke Matsunaga, All rights reserved.
"""

class Ast:
    """
    Abstruct Syntax Tree (AST) のノードを表すクラス

    * 終端ノードの場合は具体的な文字列を値に持つ．
    * 非終端ノードの場合は生成規則を持つ．
    """

class VlGen:

    def __init__(self):
        pass

    @property
    def ouput_symbol(self):
        return ['0', '1', 'x', 'X']

    @property
    def current_statte(self):
        return self.level_symbol

    @property
    def level_symbol(self):
        return ['0', '1', 'x', 'X', '?', 'b', 'B']

    @property
    def edge_symbol(self):
        return ['r', 'R', 'f', 'F', 'p', 'P', 'n', 'N', '*']
