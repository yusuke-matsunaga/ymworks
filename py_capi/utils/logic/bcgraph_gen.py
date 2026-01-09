#! /usr/bin/env python3

""" PyBcGraph を生成するスクリプト

:file: bcgraph_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import UlongArg, StringArg
from .logic_arg import BcOpTypeArg, BcEdgeArg, BcEdgeListArg, BcBvListArg


class BcGraphGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BcGraph',
                         pyname='BcGraph',
                         namespace='YM',
                         header_include_files=['ym/BcGraph.h'],
                         source_include_files=['pym/PyBcGraph.h',
                                               'pym/PyBcEdge.h',
                                               'pym/PyBcOp.h',
                                               'pym/PyBcOpType.h',
                                               'pym/PyList.h',
                                               'pym/PyUlong.h',
                                               'pym/PyUint64.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.gen_stmt('new (&my_obj->mVal) BcGraph(input_num)')
            writer.gen_return_self()
        self.add_new(func_body=new_body,
                     arg_list=[OptArg(),
                               KwdArg(),
                               UlongArg(name='input_num',
                                        cvarname='input_num',
                                        cvardefault='0')])

        self.add_dealloc()

        def get_input_num(writer):
            writer.gen_return_py_ulong('val.input_num()')
        self.add_getter('get_input_num',
                        func_body=get_input_num)
        self.add_attr('input_num',
                      getter_name='get_input_num',
                      doc_str='number of inputs')

        def get_output_num(writer):
            writer.gen_return_py_ulong('val.output_num()')
        self.add_getter('get_output_num',
                        func_body=get_output_num)
        self.add_attr('output_num',
                      getter_name='get_output_num',
                      doc_str='number of outputs')

        def get_op_num(writer):
            writer.gen_return_py_ulong('val.op_num()')
        self.add_getter('get_op_num',
                        func_body=get_op_num)
        self.add_attr('op_num',
                      getter_name='get_op_num',
                      doc_str='number OP nodes')

        def meth_op(writer):
            writer.gen_return_pyobject('PyBcOp',
                                       'val.op(op_id)')
        self.add_method('op',
                        func_body=meth_op,
                        arg_list=[UlongArg(name='op_id',
                                           cvarname='op_id')],
                        doc_str='return OP node')

        def meth_output(writer):
            writer.gen_return_pyobject('PyBcEdge',
                                       'val.output(output_id)')
        self.add_method('output',
                        func_body=meth_output,
                        arg_list=[UlongArg(name='output_id',
                                           cvarname='output_id')],
                        doc_str='return output edge')

        def meth_output_level(writer):
            writer.gen_return_py_ulong('val.output_level(output_id)')
        self.add_method('output_level',
                        func_body=meth_output_level,
                        arg_list=[UlongArg(name='output_id',
                                           cvarname='output_id')],
                        doc_str='return output level')

        def get_max_level(writer):
            writer.gen_return_py_ulong('val.max_level()')
        self.add_getter('get_max_level',
                        func_body=get_max_level)
        self.add_attr('max_level',
                      getter_name='get_max_level',
                      doc_str='max level')

        def meth_edge_level(writer):
            writer.gen_return_py_ulong('val.edge_level(edge)')
        self.add_method('edge_level',
                        func_body=meth_edge_level,
                        arg_list=[BcEdgeArg(name='edge',
                                            cvarname='edge')],
                        doc_str='return edge level')

        def meth_add_op(writer):
            writer.gen_return_py_ulong('val.add_op(type, opr_list)')
        self.add_method('add_op',
                        func_body=meth_add_op,
                        arg_list=[BcOpTypeArg(name='type',
                                              cvarname='type'),
                                  BcEdgeListArg(name='opr_list',
                                                cvarname='opr_list')],
                        doc_str='add OP')

        def meth_add_tvop(writer):
            writer.gen_return_py_ulong('val.add_tvop(tv, opr_list)')
        self.add_method('add_tvop',
                        func_body=meth_add_tvop,
                        arg_list=[BcBvListArg(name='tv',
                                              cvarname='tv'),
                                  BcEdgeListArg(name='opr_list',
                                                cvarname='opr_list')],
                        doc_str='add TVOP')

        def meth_add_output(writer):
            writer.gen_return_py_ulong('val.add_output(edge)')
        self.add_method('add_output',
                        func_body=meth_add_output,
                        arg_list=[BcEdgeArg(name='edge',
                                            cvarname='edge')],
                        doc_str='add output')

        def meth_add_zero_output(writer):
            writer.gen_return_py_ulong('val.add_zero_output()')
        self.add_method('add_zero_output',
                        func_body=meth_add_zero_output,
                        doc_str='add output of constant 0')

        def meth_add_one_output(writer):
            writer.gen_return_py_ulong('val.add_one_output()')
        self.add_method('add_one_output',
                        func_body=meth_add_one_output,
                        doc_str='add output of constant 1')

        self.add_conv('default')
        self.add_deconv('default')
