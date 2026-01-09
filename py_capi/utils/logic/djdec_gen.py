#! /usr/bin/env python3

""" PyDjDec を生成するスクリプト

:file: djdec_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, RawObjArg


class DjDecGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='DjDec',
                         pyname='DjDec',
                         namespace='YM',
                         header_include_files=['ym/DjDec.h'],
                         source_include_files=['pym/PyDjDec.h',
                                               'pym/PyBdd.h',
                                               'pym/PyTvFunc.h',
                                               'pym/PyBcGraph.h'])

        def meth_decompose(writer):
            writer.gen_vardecl(typename='Bdd',
                               varname='bdd_func')
            with writer.gen_if_block('PyBdd::FromPyObject(func_obj, bdd_func)'):
                writer.gen_return_pyobject('PyBcGraph',
                                           'DjDec::decompose(bdd_func)')
            writer.gen_vardecl(typename='TvFunc',
                               varname='tv_func')
            with writer.gen_if_block('PyTvFunc::FromPyObject(func_obj, tv_func)'):
                writer.gen_return_pyobject('PyBcGraph',
                                           'DjDec::decompose(tv_func)')
            writer.gen_type_error('"argument 1 must be either Bdd or TvFunc"')
        self.add_static_method('decompose',
                               func_body=meth_decompose,
                               arg_list=[RawObjArg(name='func',
                                                   cvarname='func_obj')],
                               doc_str='do Disjoint Support Decomposition')
