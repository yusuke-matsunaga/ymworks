#! /usr/bin/env python3

""" PyTruthTable を生成するスクリプト

:file: truthtable_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import UlongArg, StringArg
from .logic_arg import Val3Arg


class TruthTableGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='TruthTable',
                         pyname='TruthTable',
                         namespace='YM',
                         header_include_files=['ym/TruthTable.h'],
                         source_include_files=['pym/PyTruthTable.h',
                                               'pym/PyVal3.h',
                                               'pym/PyString.h',
                                               'pym/PyUlong.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.gen_stmt('new (&my_obj->mVal) TruthTable(input_num, output_num)')
            writer.gen_return_self()
        self.add_new(func_body=new_body,
                     arg_list=[KwdArg(),
                               UlongArg(name='input_num',
                                        cvarname='input_num'),
                               UlongArg(name='output_num',
                                        cvarname='output_num')])

        self.add_dealloc()

        def meth_read_file(writer):
            writer.gen_return_pyobject('PyTruthTable',
                                       'TruthTable::read_file(filename)')
        self.add_static_method('read_file',
                               func_body=meth_read_file,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               doc_str='make TruthTable from file')

        def meth_from_bin(writer):
            writer.gen_return_pyobject('PyTruthTable',
                                       'TruthTable::from_bin(str)')
        self.add_static_method('from_bin',
                               func_body=meth_from_bin,
                               arg_list=[StringArg(name='str',
                                                   cvarname='str')],
                               doc_str='name TruthTable from BIN string')

        def meth_from_hex(writer):
            writer.gen_return_pyobject('PyTruthTable',
                                       'TruthTable::from_hex(str)')
        self.add_static_method('from_hex',
                               func_body=meth_from_hex,
                               arg_list=[StringArg(name='str',
                                                   cvarname='str')],
                               doc_str='name TruthTable from HEX string')

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

        def meth_get_value(writer):
            writer.gen_return_pyobject('PyVal3',
                                       'val.get_value(i_index, o_index)')
        self.add_method('get_value',
                        func_body=meth_get_value,
                        arg_list=[KwdArg(),
                                  UlongArg(name='i_index',
                                           cvarname='i_index'),
                                  UlongArg(name='o_index',
                                           cvarname='o_index')],
                        doc_str='get value')

        def meth_set_value(writer):
            writer.gen_stmt('val.set_value(i_index, o_index, v)')
            writer.gen_return_py_none()
        self.add_method('set_value',
                        func_body=meth_set_value,
                        arg_list=[KwdArg(),
                                  UlongArg(name='i_index',
                                           cvarname='i_index'),
                                  UlongArg(name='o_index',
                                           cvarname='o_index'),
                                  Val3Arg(name='val',
                                          cvarname='v')],
                        doc_str='set value')

        def meth_bin_str(writer):
            writer.gen_return_py_string('val.bin_str()')
        self.add_method('bin_str',
                        func_body=meth_bin_str,
                        doc_str='return BIN format string')

        def meth_hex_str(writer):
            writer.gen_return_py_string('val.hex_str()')
        self.add_method('hex_str',
                        func_body=meth_bin_str,
                        doc_str='return HEX format string')

        self.add_conv('default')
        self.add_deconv('default')
