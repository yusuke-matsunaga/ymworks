#! /usr/bin/env python3

""" PyClibBusType を生成するスクリプト

:file: clibbustype_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg


class ClibBusTypeGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibBusType',
                         pyname='ClibBusType',
                         namespace='YM',
                         header_include_files=['ym/ClibBusType.h'],
                         source_include_files=['pym/PyClibBusType.h',
                                               'pym/PyString.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc('default')

        def get_name(writer):
            return writer.gen_return_py_string('val.name()')
        self.add_getter('get_name',
                        func_body=get_name)
        self.add_attr('name',
                      getter_name='get_name',
                      doc_str='name')

        def get_base_type(writer):
            return writer.gen_return_py_string('"ArrayType"')
        self.add_getter('get_base_type',
                        func_body=get_base_type)
        self.add_attr('base_type',
                      getter_name='get_base_type',
                      doc_str='base type(\\\"ArrayType\\\" only)')

        def get_data_type(writer):
            return writer.gen_return_py_string('"BitType"')
        self.add_getter('get_data_type',
                        func_body=get_data_type)
        self.add_attr('data_type',
                      getter_name='get_data_type',
                      doc_str='data type')

        def get_bit_width(writer):
            return writer.gen_return_py_ulong('val.bit_width()')
        self.add_getter('get_bit_width',
                        func_body=get_bit_width)
        self.add_attr('bit_width',
                      getter_name='get_bit_width',
                      doc_str='bit witdh')

        def get_bit_from(writer):
            return writer.gen_return_py_ulong('val.bit_from()')
        self.add_getter('get_bit_from',
                        func_body=get_bit_from)
        self.add_attr('bit_from',
                      getter_name='get_bit_from',
                      doc_str='bit from position')

        def get_bit_to(writer):
            return writer.gen_return_py_ulong('val.bit_to()')
        self.add_getter('get_bit_to',
                        func_body=get_bit_to)
        self.add_attr('bit_to',
                      getter_name='get_bit_to',
                      doc_str='bit to position')

        def get_downto(writer):
            return writer.gen_return_py_bool('val.downto()')
        self.add_getter('get_downto',
                        func_body=get_downto)
        self.add_attr('downto',
                      getter_name='get_downto',
                      doc_str='True if down-to direction')

        def meth_is_valid(writer):
            return writer.gen_return_py_bool('val.is_valid()')
        self.add_method('is_valid',
                        func_body=meth_is_valid,
                        doc_str='True if valid')

        def meth_is_invalid(writer):
            return writer.gen_return_py_bool('val.is_invalid()')
        self.add_method('is_invalid',
                        func_body=meth_is_invalid,
                        doc_str='True if invalid')

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
