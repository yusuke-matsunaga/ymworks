#! /usr/bin/env python3

""" PySatCondBlock を生成するスクリプト

:file: satcondblock_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, RawObjArg
from .sat_arg import SatSolverRefArg, SatLiteralArg


class SatCondBlockGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='SatCondBlock',
                         pyname='SatCondBlock',
                         namespace='YM',
                         header_include_files=['ym/SatCondBlock.h'],
                         source_include_files=['pym/PySatCondBlock.h',
                                               'pym/PySatSolver.h',
                                               'pym/PySatLiteral.h'])

        def meth_enter(writer):
            writer.gen_return_py_none()
        self.add_method('__enter__',
                        func_body=meth_enter)

        def meth_exit(writer):
            writer.gen_return_py_none()
        self.add_method('__exit__',
                        func_body=meth_exit,
                        arg_list=[RawObjArg(name='ex_type',
                                            cvarname='ex_type_obj'),
                                  RawObjArg(name='ex_value',
                                            cvarname='ex_value_obj'),
                                  RawObjArg(name='trace',
                                            cvarname='trace_obj')])

        self.add_conv('default', move=True)
