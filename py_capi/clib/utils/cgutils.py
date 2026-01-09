#! /usr/bin/env python3

"""CodeGen で用いられる補助クラスの定義

:file: cgutils.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

    
class Line:
    """一行を表すクラス"""
    def __init__(self, body, *, ignore_indent=False):
        self.body = body
        self.ignore_indent = ignore_indent

    def gen(self, fout, indent):
        """内容を出力する．"""
        if self.ignore_indent:
            indent = 0
        Line.write_line(self.body, fout, indent)

    @staticmethod
    def write_line(body, fout, indent):
        sp = ' ' * indent
        fout.write(f'{sp}{body}\n')


class Text:
    """複数行を表すクラス"""
    def __init__(self, body):
        self.body = body

    def gen(self, fout, indent):
        """内容を出力する．"""
        fout.write(self.body)
        

class Block:
    """ブロック構造を表すクラス"""
    def __init__(self, body, *, block_symbol=["{", "}"], indent_delta=2):
        self.body = body
        self.block_symbol = block_symbol
        self.indent_delta = indent_delta

    def gen(self, fout, indent):
        """内容を出力する．"""
        if self.block_symbol[0] is not None:
            Line.write_line(f'{self.block_symbol[0]}', fout, indent)
        for elem in self.body:
            if elem is not None:
                elem.gen(fout, indent + self.indent_delta)
        if self.block_symbol[1] is not None:
            Line.write_line(f'{self.block_symbol[1]}', fout, indent)

    @staticmethod
    def new_list(body):
        """リストブロックを作る．"""
        return Block(body,
                     block_symbol=[None, None],
                     indent_delta=0)
    
    @staticmethod
    def new_namespace(name, body):
        """名前空間のブロックを作る．"""
        return Block(body,
                     block_symbol=[f"BEGIN_NAMESPACE_{name}\n",
                                   f"\nEND_NAMESPaCE_{name}"],
                     indent_delta=0)

    @staticmethod
    def new_nonamespace(body):
        """匿名の名前空間のブロックを作る．"""
        return Block(body,
                     block_symbol=["BEGIN_NONAMESPACE\n",
                                   "\nEND_NONAMESPACE"])

    
class FuncDef:
    """関数定義を表すクラス"""
    def __init__(self, name, *,
                 return_type, param_list=[], body=[]):
        self.name = Line(f'{name}(')
        self.return_type = return_type
        self.head = Block(param_list,
                          block_symbol=[f"{name}(", ")"])
        self.body = Block(body)

    def gen(self, fout, indent):
        self.return_type.gen(fout, indent)
        self.head.gen(fout, indent)
        self.body.gen(fout, indent)
