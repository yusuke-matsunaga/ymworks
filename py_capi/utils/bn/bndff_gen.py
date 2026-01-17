#! /usr/bin/env python3

""" PyBnDff を生成するスクリプト

:file: bndff_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import IntArg, BoolArg, RawObjArg
from mk_py_capi import OptArg, KwdArg


class BnDffGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BnDff',
                         pyname='BnDff',
                         namespace='YM',
                         header_include_files=['ym/BnDff.h'],
                         source_include_files=['pym/PyBnDff.h',
                                               'pym/PyBnNode.h',
                                               'pym/PyInt.h',
                                               'pym/PyString.h'])

        self.add_dealloc('default')

        def get_id(writer):
            writer.gen_return_py_int('val.id()')
        self.add_getter('get_id',
                        func_body=get_id)
        self.add_attr('id',
                      getter_name='get_id',
                      doc_str="ID")

        def get_output(writer):
            writer.gen_return_pyobject('PyBnNode', 'val.output()')
        self.add_getter('get_output',
                        func_body=get_output)
        self.add_attr('output',
                      getter_name='get_output',
                      doc_str="output node")

        def get_input(writer):
            writer.gen_return_pyobject('PyBnNode', 'val.input()')
        self.add_getter('get_input',
                        func_body=get_input)
        self.add_attr('input',
                      getter_name='get_input',
                      doc_str="input node")

        def get_reset_val(writer):
            writer.gen_return_py_string('std::string{val.reset_val()}')
        self.add_getter('get_reset_val',
                        func_body=get_reset_val)
        self.add_attr('reset_val',
                      getter_name='get_reset_val',
                      doc_str="reset value")

        self.add_conv('default')
        self.add_deconv('default')
