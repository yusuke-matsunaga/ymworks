#! /usr/bin/env python3

""" PyBcOp を生成するスクリプト

:file: bcop_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import UlongArg


class BcOpGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BcOp',
                         pyname='BcOp',
                         namespace='YM',
                         header_include_files=['ym/BcGraph.h'],
                         source_include_files=['pym/PyBcOp.h',
                                               'pym/PyBcEdge.h',
                                               'pym/PyBcOpType.h',
                                               'pym/PyString.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc()

        def get_id(writer):
            writer.gen_return_py_ulong('val.id()')
        self.add_getter('get_id',
                        func_body=get_id)
        self.add_attr('id',
                      getter_name='get_id',
                      doc_str='ID')

        def get_type(writer):
            writer.gen_return_pyobject('PyBcOpType',
                                       'val.type()')
        self.add_getter('get_type',
                        func_body=get_type)
        self.add_attr('type',
                      getter_name='get_type',
                      doc_str='type(AND, OR, XOR, TV)')

        def get_operand_num(writer):
            writer.gen_return_py_ulong('val.operand_num()')
        self.add_getter('get_operand_num',
                        func_body=get_operand_num)
        self.add_attr('operand_num',
                      getter_name='get_operand_num',
                      doc_str='number of operands')

        def meth_operand(writer):
            writer.gen_return_pyobject('PyBcEdge',
                                       'val.operand(pos)')
        self.add_method('operand',
                        func_body=meth_operand,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return operand edge')

        def get_level(writer):
            writer.gen_return_py_ulong('val.level()')
        self.add_getter('get_level',
                        func_body=get_level)
        self.add_attr('level',
                      getter_name='get_level',
                      doc_str='level')

        self.add_richcompare('eq_default')

        def meth_to_string(writer):
            writer.gen_return_py_string('val.to_string()')
        self.add_method('to_string',
                        func_body=meth_to_string,
                        doc_str='return string representation')

        self.add_conv('default')
        self.add_deconv('default')
