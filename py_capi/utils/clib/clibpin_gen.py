#! /usr/bin/env python3

""" PyClibPin を生成するスクリプト

:file: clibpin_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import LongArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg


class ClibPinGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibPin',
                         pyname='ClibPin',
                         namespace='YM',
                         header_include_files=['ym/ClibPin.h'],
                         source_include_files=['pym/PyClibPin.h',
                                               'pym/PyClibDirection.h',
                                               'pym/PyClibCapacitance.h',
                                               'pym/PyExpr.h',
                                               'pym/PyString.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc('default')

        def get_name(writer):
            writer.gen_return_py_string('val.name()')
        self.add_getter('get_name',
                        func_body=get_name)
        self.add_attr('name',
                      getter_name='get_name',
                      doc_str='name')

        def get_direction(writer):
            writer.gen_return_pyobject('PyClibDirection', 'val.direction()')
        self.add_getter('get_direction',
                        func_body=get_direction)
        self.add_attr('direction',
                      getter_name='get_direction',
                      doc_str='direction')

        def meth_is_input(writer):
            writer.gen_return_py_bool('val.is_input()')
        self.add_method('is_input',
                        func_body=meth_is_input,
                        doc_str='True if INPUT')

        def meth_is_output(writer):
            writer.gen_return_py_bool('val.is_output()')
        self.add_method('is_output',
                        func_body=meth_is_output,
                        doc_str='True if OUTPUT')

        def meth_is_inout(writer):
            writer.gen_return_py_bool('val.is_inout()')
        self.add_method('is_inout',
                        func_body=meth_is_inout,
                        doc_str='True if INOUT')

        def meth_is_internal(writer):
            writer.gen_return_py_bool('val.is_internal()')
        self.add_method('is_internal',
                        func_body=meth_is_internal,
                        doc_str='True if INTERNAL')

        def get_pin_id(writer):
            writer.gen_return_py_ulong('val.pin_id()')
        self.add_getter('get_pin_id',
                        func_body=get_pin_id)
        self.add_attr('pin_id',
                      getter_name='get_pin_id',
                      doc_str='pin ID')

        def get_input_id(writer):
            writer.gen_return_py_ulong('val.input_id()')
        self.add_getter('get_input_id',
                        func_body=get_input_id)
        self.add_attr('input_id',
                      getter_name='get_input_id',
                      doc_str='input ID')

        def get_capacitance(writer):
            writer.gen_return_pyobject('PyClibCapacitance',
                                       'val.capacitance()')
        self.add_getter('get_capacitance',
                        func_body=get_capacitance)
        self.add_attr('capacitance',
                      getter_name='get_capacitance',
                      doc_str='capacitance')

        def get_rise_capacitance(writer):
            writer.gen_return_pyobject('PyClibCapacitance',
                                       'val.rise_capacitance()')
        self.add_getter('get_rise_capacitance',
                        func_body=get_rise_capacitance)
        self.add_attr('rise_capacitance',
                      getter_name='get_rise_capacitance',
                      doc_str='rise capacitance')

        def get_fall_capacitance(writer):
            writer.gen_return_pyobject('PyClibCapacitance',
                                       'val.fall_capacitance()')
        self.add_getter('get_fall_capacitance',
                        func_body=get_fall_capacitance)
        self.add_attr('fall_capacitance',
                      getter_name='get_fall_capacitance',
                      doc_str='fall capacitance')

        def get_output_id(writer):
            writer.gen_return_py_ulong('val.output_id()')
        self.add_getter('get_output_id',
                        func_body=get_output_id)
        self.add_attr('output_id',
                      getter_name='get_output_id',
                      doc_str='output ID')

        def get_max_fanout(writer):
            writer.gen_return_pyobject('PyClibCapacitance',
                                       'val.max_fanout()')
        self.add_getter('get_max_fanout',
                        func_body=get_max_fanout)
        self.add_attr('max_fanout',
                      getter_name='get_max_fanout',
                      doc_str='max fanout')

        def get_min_fanout(writer):
            writer.gen_return_pyobject('PyClibCapacitance',
                                       'val.min_fanout()')
        self.add_getter('get_min_fanout',
                        func_body=get_min_fanout)
        self.add_attr('min_fanout',
                      getter_name='get_min_fanout',
                      doc_str='min fanout')

        def get_max_capacitance(writer):
            writer.gen_return_pyobject('PyClibCapacitance',
                                       'val.max_capacitance()')
        self.add_getter('get_max_capacitance',
                        func_body=get_max_capacitance)
        self.add_attr('max_capacitance',
                      getter_name='get_max_capacitance',
                      doc_str='max capacitance')

        def get_min_capacitance(writer):
            writer.gen_return_pyobject('PyClibCapacitance',
                                       'val.min_capacitance()')
        self.add_getter('get_min_capacitance',
                        func_body=get_min_capacitance)
        self.add_attr('min_capacitance',
                      getter_name='get_min_capacitance',
                      doc_str='min capacitance')

        def get_function(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'val.function()')
        self.add_getter('get_function',
                        func_body=get_function)
        self.add_attr('function',
                      getter_name='get_function',
                      doc_str='function')

        def get_tristate(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'val.tristate()')
        self.add_getter('get_tristate',
                        func_body=get_tristate)
        self.add_attr('tristate',
                      getter_name='get_tristate',
                      doc_str='tristate condition')

        def get_internal_id(writer):
            writer.gen_return_py_ulong('val.internal_id()')
        self.add_getter('get_internal_id',
                        func_body=get_internal_id)
        self.add_attr('internal_id',
                      getter_name='get_internal_id',
                      doc_str='internal ID')

        self.add_conv('default')
        self.add_deconv('default')
