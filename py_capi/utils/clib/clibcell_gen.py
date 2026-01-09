#! /usr/bin/env python3

""" PyClibCell を生成するスクリプト

:file: clibcell_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import LongArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg
from .clib_arg import ClibTimingSenseArg


class ClibCellGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibCell',
                         pyname='ClibCell',
                         namespace='YM',
                         header_include_files=['ym/ClibCell.h',
                                               'ym/ClibBus.h'],
                         source_include_files=['pym/PyClibCell.h',
                                               'pym/PyClibCellLibrary.h',
                                               'pym/PyClibCellGroup.h',
                                               'pym/PyClibCellType.h',
                                               'pym/PyClibCPV.h',
                                               'pym/PyClibPin.h',
                                               'pym/PyClibBus.h',
                                               'pym/PyClibBundle.h',
                                               'pym/PyClibTiming.h',
                                               'pym/PyClibTimingSense.h',
                                               'pym/PyClibList.h',
                                               'pym/PyClibArea.h',
                                               'pym/PyExpr.h',
                                               'pym/PyString.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc('default')

        def get_library(writer):
            return writer.gen_return_pyobject('PyClibCellLibrary',
                                              'val.library()')
        self.add_getter('get_library',
                        func_body=get_library)
        self.add_attr('library',
                      getter_name='get_library',
                      doc_str='cell library')

        def get_group(writer):
            return writer.gen_return_pyobject('PyClibCellGroup',
                                              'val.group()')
        self.add_getter('get_group',
                        func_body=get_group)
        self.add_attr('group',
                      getter_name='get_group',
                      doc_str='cell group')

        def get_name(writer):
            return writer.gen_return_py_string('val.name()')
        self.add_getter('get_name',
                        func_body=get_name)
        self.add_attr('name',
                      getter_name='get_name',
                      doc_str='name')

        def get_area(writer):
            return writer.gen_return_pyobject('PyClibArea', 'val.area()')
        self.add_getter('get_area',
                        func_body=get_area)
        self.add_attr('area',
                      getter_name='get_area',
                      doc_str='area')

        def get_pin_num(writer):
            return writer.gen_return_py_ulong('val.pin_num()')
        self.add_getter('get_pin_num',
                        func_body=get_pin_num)
        self.add_attr('pin_num',
                      getter_name='get_pin_num',
                      doc_str='number of pins')

        def meth_pin(writer):
            with writer.gen_if_block('PyUlong::Check(arg_obj)'):
                writer.gen_auto_assign('pos', 'PyUlong::Get(arg_obj)')
                writer.gen_return_pyobject('PyClibPin', 'val.pin(pos)')
            with writer.gen_elseif_block('PyString::Check(arg_obj)'):
                writer.gen_auto_assign('name', 'PyString::Get(arg_obj)')
                writer.gen_return_pyobject('PyClibPin', 'val.pin(name)')
            with writer.gen_else_block():
                writer.gen_type_error('"1st argument should be integer or string"')
        self.add_method('pin',
                        func_body=meth_pin,
                        arg_list=[RawObjArg(cvarname='arg_obj')],
                        doc_str='return pin')

        def meth_pin_list(writer):
            writer.gen_return_pyobject('PyClibPinList',
                                       'val.pin_list()')
        self.add_method('meth_pin_list',
                        func_body=meth_pin_list,
                        doc_str='return list of pins')

        def get_input_num(writer):
            writer.gen_return_py_ulong('val.input_num()')
        self.add_getter('get_input_num',
                        func_body=get_input_num)
        self.add_attr('input_num',
                      getter_name='get_input_num',
                      doc_str='number of input pins')

        def meth_input(writer):
            writer.gen_return_pyobject('PyClibPin', 'val.input(pos)')
        self.add_method('input',
                        func_body=meth_input,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return input pin')

        def meth_input_list(writer):
            writer.gen_return_pyobject('PyClibPinList',
                                       'val.input_list()')
        self.add_method('meth_input_list',
                        func_body=meth_input_list,
                        doc_str='return list of input pins')

        def get_output_num(writer):
            writer.gen_return_py_ulong('val.output_num()')
        self.add_getter('get_output_num',
                        func_body=get_output_num)
        self.add_attr('output_num',
                      getter_name='get_output_num',
                      doc_str='number of output pins')

        def meth_output(writer):
            writer.gen_return_pyobject('PyClibPin',
                                       'val.output(pos)')
        self.add_method('output',
                        func_body=meth_output,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return output pin')

        def meth_output_list(writer):
            writer.gen_return_pyobject('PyClibPinList',
                                       'val.output_list()')
        self.add_method('meth_output_list',
                        func_body=meth_output_list,
                        doc_str='return list of output pins')

        def get_inout_num(writer):
            writer.gen_return_py_ulong('val.inout_num()')
        self.add_getter('get_inout_num',
                        func_body=get_inout_num)
        self.add_attr('inout_num',
                      getter_name='get_inout_num',
                      doc_str='number of inout pins')

        def meth_inout(writer):
            writer.gen_return_pyobject('PyClibPin',
                                       'val.inout(pos)')
        self.add_method('inout',
                        func_body=meth_inout,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return inout pin')

        def meth_inout_list(writer):
            writer.gen_return_pyobject('PyClibPinList',
                                       'val.inout_list()')
        self.add_method('meth_inout_list',
                        func_body=meth_inout_list,
                        doc_str='return list of inout pins')

        def get_input2_num(writer):
            writer.gen_return_py_ulong('val.input2_num()')
        self.add_getter('get_input2_num',
                        func_body=get_input2_num)
        self.add_attr('input2_num',
                      getter_name='get_input2_num',
                      doc_str='number of input/inout pins')

        def meth_input2(writer):
            writer.gen_return_pyobject('PyClibPin',
                                       'val.input2(pos)')
        self.add_method('input2',
                        func_body=meth_input2,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return input/inout pin')

        def get_output2_num(writer):
            writer.gen_return_py_ulong('val.output2_num()')
        self.add_getter('get_output2_num',
                        func_body=get_output2_num)
        self.add_attr('output2_num',
                      getter_name='get_output2_num',
                      doc_str='number of output/inout pins')

        def meth_output2(writer):
            writer.gen_return_pyobject('PyClibPin',
                                       'val.output2(pos)')
        self.add_method('output2',
                        func_body=meth_output2,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return output/inout pin')

        def get_internal_num(writer):
            writer.gen_return_py_ulong('val.internal_num()')
        self.add_getter('get_internal_num',
                        func_body=get_internal_num)
        self.add_attr('internal_num',
                      getter_name='get_internal_num',
                      doc_str='number of internal pins')

        def meth_internal(writer):
            writer.gen_return_pyobject('PyClibPin',
                                       'val.internal(pos)')
        self.add_method('internal',
                        func_body=meth_internal,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return internal pin')

        def meth_internal_list(writer):
            writer.gen_return_pyobject('PyClibPinList',
                                       'val.internal_list()')
        self.add_method('meth_internal_list',
                        func_body=meth_internal_list,
                        doc_str='return list of internal pins')

        def get_bus_num(writer):
            writer.gen_return_py_ulong('val.bus_num()')
        self.add_getter('get_bus_num',
                        func_body=get_bus_num)
        self.add_attr('bus_num',
                      getter_name='get_bus_num',
                      doc_str='number of buses')

        def meth_bus(writer):
            writer.gen_return_pyobject('PyClibBus',
                                       'val.bus(pos)')
        self.add_method('bus',
                        func_body=meth_bus,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return bus')

        def meth_bus_list(writer):
            writer.gen_return_pyobject('PyClibBusList',
                                       'val.bus_list()')
        self.add_method('meth_bus_list',
                        func_body=meth_bus_list,
                        doc_str='return list of buses')

        def get_bundle_num(writer):
            writer.gen_return_py_ulong('val.bundle_num()')
        self.add_getter('get_bundle_num',
                        func_body=get_bundle_num)
        self.add_attr('bundle_num',
                      getter_name='get_bundle_num',
                      doc_str='number of bundles')

        def meth_bundle(writer):
            writer.gen_return_pyobject('PyClibBundle',
                                       'val.bundle(pos)')
        self.add_method('bundle',
                        func_body=meth_bundle,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return bundle')

        def meth_bundle_list(writer):
            writer.gen_return_pyobject('PyClibBundleList',
                                       'val.bundle_list()')
        self.add_method('meth_bundle_list',
                        func_body=meth_bundle_list,
                        doc_str='return list of bundles')

        def meth_timing_list(writer):
            writer.gen_return_pyobject('PyClibTimingList',
                                       'val.timing_list(ipos, opos, sense)')
        self.add_method('timing_list',
                        func_body=meth_timing_list,
                        arg_list=[KwdArg(),
                                  UlongArg(name='ipos',
                                           cvarname='ipos'),
                                  UlongArg(name='opos',
                                           cvarname='opos'),
                                  ClibTimingSenseArg(name='sense',
                                                     cvarname='sense')],
                        doc_str='return list of timing information')

        def get_type(writer):
            writer.gen_return_pyobject('PyClibCellType',
                                       'val.type()')
        self.add_getter('get_type',
                        func_body=get_type)
        self.add_attr('type',
                      getter_name='get_type',
                      doc_str='type(Logic, FF, Latch, FSM)')

        def meth_is_logic(writer):
            writer.gen_return_py_bool('val.is_logic()')
        self.add_method('is_logic',
                        func_body=meth_is_logic,
                        doc_str='True if LOGIC cell')

        def meth_is_ff(writer):
            writer.gen_return_py_bool('val.is_ff()')
        self.add_method('is_ff',
                        func_body=meth_is_ff,
                        doc_str='True if FF cell')

        def meth_is_latch(writer):
            writer.gen_return_py_bool('val.is_latch()')
        self.add_method('is_latch',
                        func_body=meth_is_latch,
                        doc_str='True if LATCH cell')

        def meth_has_logic(writer):
            with writer.gen_if_block('pin_id == -1'):
                writer.gen_return_py_bool('val.has_logic()')
            with writer.gen_else_block():
                writer.gen_return_py_bool('val.has_logic(pin_id)')
        self.add_method('has_logic',
                        func_body=meth_has_logic,
                        arg_list=[OptArg(),
                                  LongArg(name='pin_id',
                                          cvarname='pin_id',
                                          cvardefault=-1)],
                        doc_str='True if the output has logic expression')

        def meth_logic_expr(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'val.logic_expr(pin_id)')
        self.add_method('logic_expr',
                        func_body=meth_logic_expr,
                        arg_list=[UlongArg(name='pin_id',
                                           cvarname='pin_id')],
                        doc_str='return logic expression of the output')

        def meth_has_tristate(writer):
            writer.gen_return_py_bool('val.has_tristate(pin_id)')
        self.add_method('has_tristate',
                        func_body=meth_has_tristate,
                        arg_list=[UlongArg(name='pin_id',
                                          cvarname='pin_id')],
                        doc_str='True if the output has tristate expression')

        def meth_tristate_expr(writer):
            writer.gen_return_pyobject('PyExpr', 'val.tristate_expr(pin_id)')
        self.add_method('tristate_expr',
                        func_body=meth_tristate_expr,
                        arg_list=[UlongArg(name='pin_id',
                                           cvarname='pin_id')],
                        doc_str='return tristate expression of the output')

        def get_qvar1(writer):
            writer.gen_return_py_string('val.qvar1()')
        self.add_getter('get_qvar1',
                        func_body=get_qvar1)
        self.add_attr('qvar1',
                      getter_name='get_qvar1',
                      doc_str='qvar1')

        def get_qvar2(writer):
            writer.gen_return_py_string('val.qvar2()')
        self.add_getter('get_qvar2',
                        func_body=get_qvar2)
        self.add_attr('qvar2',
                      getter_name='get_qvar2',
                      doc_str='qvar2')

        def meth_has_clear(writer):
            writer.gen_return_py_bool('val.has_clear()')
        self.add_method('has_clear',
                        func_body=meth_has_clear,
                        doc_str='True if having CLEAR pin')

        def get_clear_expr(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'val.clear_expr()')
        self.add_getter('get_clear_expr',
                        func_body=get_clear_expr)
        self.add_attr('clear_expr',
                      getter_name='get_clear_expr',
                      doc_str='clear expression')

        def meth_has_preset(writer):
            writer.gen_return_py_bool('val.has_preset()')
        self.add_method('has_preset',
                        func_body=meth_has_preset,
                        doc_str='True if having PRESET pin')

        def get_preset_expr(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'val.preset_expr()')
        self.add_getter('get_preset_expr',
                        func_body=get_preset_expr)
        self.add_attr('preset_expr',
                      getter_name='get_preset_expr',
                      doc_str='preset expression')

        def get_cpv1(writer):
            writer.gen_return_pyobject('PyClibCPV',
                                       'val.clear_preset_var1()')
        self.add_getter('get_cpv1',
                        func_body=get_cpv1)
        self.add_attr('clear_preset_var1',
                      getter_name='get_cpv1',
                      doc_str='clear preset var1')

        def get_cpv2(writer):
            writer.gen_return_pyobject('PyClibCPV',
                                       'val.clear_preset_var2()')
        self.add_getter('get_cpv2',
                        func_body=get_cpv2)
        self.add_attr('clear_preset_var2',
                      getter_name='get_cpv2',
                      doc_str='clear preset var2')

        def get_clock_expr(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'val.clock_expr()')
        self.add_getter('get_clock_expr',
                        func_body=get_clock_expr)
        self.add_attr('clock_expr',
                      getter_name='get_clock_expr',
                      doc_str='clock expression')

        def get_clock2_expr(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'val.clock2_expr()')
        self.add_getter('get_clock2_expr',
                        func_body=get_clock2_expr)
        self.add_attr('clock2_expr',
                      getter_name='get_clock2_expr',
                      doc_str='clock2 expression')

        def get_next_state_expr(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'val.next_state_expr()')
        self.add_getter('get_next_state_expr',
                        func_body=get_next_state_expr)
        self.add_attr('next_state_expr',
                      getter_name='get_next_state_expr',
                      doc_str='next state expression')

        def get_enable_expr(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'val.enable_expr()')
        self.add_getter('get_enable_expr',
                        func_body=get_enable_expr)
        self.add_attr('enable_expr',
                      getter_name='get_enable_expr',
                      doc_str='enable expression')

        def get_enable2_expr(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'val.enable2_expr()')
        self.add_getter('get_enable2_expr',
                        func_body=get_enable2_expr)
        self.add_attr('enable2_expr',
                      getter_name='get_enable2_expr',
                      doc_str='enable2 expression')

        def get_data_in_expr(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'val.data_in_expr()')
        self.add_getter('get_data_in_expr',
                        func_body=get_data_in_expr)
        self.add_attr('data_in_expr',
                      getter_name='get_data_in_expr',
                      doc_str='data-in expression')

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
