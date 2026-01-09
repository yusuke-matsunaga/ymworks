#! /usr/bin/env python3

""" PyBcOpType を生成するスクリプト

:file: bcoptype_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import EnumGen, EnumInfo


class BcOpTypeGen(EnumGen):

    def __init__(self):
        super().__init__(classname='BcOpType',
                         pyname='BcOpType',
                         namespace='YM',
                         enum_list=[EnumInfo('BcOpType::AND', 'AND', 'AND'),
                                    EnumInfo('BcOpType::OR', 'OR', 'OR'),
                                    EnumInfo('BcOpType::XOR', 'XOR', 'XOR'),
                                    EnumInfo('BcOpType::TV', 'TV', 'TV')],
                         none_value='BcOpType::NONE',
                         header_include_files=['ym/bc.h'],
                         source_include_files=['pym/PyBcOpType.h',
                                               'pym/PyString.h'])
