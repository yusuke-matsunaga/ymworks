#! /usr/bin/env python3

""" CMakeLists.txt を出力するクラス

:file: cmwriter.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from .writer import Writer


class CmWriter(Writer):

    def __init__(self, *, fout):
        super().__init__(fout=fout)

    def gen_include_directories(self, dir_list):
        if len(dir_list) > 0:
            self.write_line('include_directories(')
            self.indent_inc()
            for inc_dir in dir_list:
                self.write_line(inc_dir)
            self.indent_dec()
            self.write_line(')')

    def gen_add_subdirectory(self, sub_dir):
        self.write_line(f'add_subdirectory( {sub_dir} )')
