#! /usr/bin/env python3

""" PyClibPatGraph を生成するスクリプト

:file: clibpatgraph_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import LongArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg


class ClibPatGraphGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibPatGraph',
                         pyname='ClibPatGraph',
                         namespace='YM',
                         header_include_files=['ym/ClibPatGraph.h'],
                         source_include_files=['pym/PyClibPatGraph.h',
                                               'pym/PyClibCellClass.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc('default')

        def get_rep_class(writer):
            writer.gen_return_pyobject('PyClibCellClass',
                                       'val.rep_class()')
        self.add_getter('get_rep_class',
                        func_body=get_rep_class)
        self.add_attr('rep_class',
                      getter_name='get_rep_class',
                      doc_str='representative class')

        def get_root_id(writer):
            writer.gen_return_py_ulong('val.root_id()')
        self.add_getter('get_root_id',
                        func_body=get_root_id)
        self.add_attr('root_id',
                      getter_name='get_root_id',
                      doc_str='root ID')

        def get_root_inv(writer):
            writer.gen_return_py_bool('val.root_inv()')
        self.add_getter('get_root_inv',
                        func_body=get_root_inv)
        self.add_attr('root_inv',
                      getter_name='get_root_inv',
                      doc_str='True if root is inverted')

        def get_input_num(writer):
            writer.gen_return_py_ulong('val.input_num()')
        self.add_getter('get_input_num',
                        func_body=get_input_num)
        self.add_attr('input_num',
                      getter_name='get_input_num',
                      doc_str='number of inputs')

        def get_edge_num(writer):
            writer.gen_return_py_ulong('val.edge_num()')
        self.add_getter('get_edge_num',
                        func_body=get_edge_num)
        self.add_attr('edge_num',
                      getter_name='get_edge_num',
                      doc_str='number of edges')

        def meth_edge(writer):
            writer.gen_return_py_ulong('val.edge(pos)')
        self.add_method('edge',
                        func_body=meth_edge,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return EDGE ID')

        def meth_is_valid(writer):
            writer.gen_return_py_bool('val.is_valid()')
        self.add_method('is_valid',
                        func_body=meth_is_valid,
                        doc_str='True if valid')

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
