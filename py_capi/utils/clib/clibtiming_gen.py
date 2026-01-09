#! /usr/bin/env python3

""" PyClibTiming を生成するスクリプト

:file: clibtiming_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import LongArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg
from .clib_arg import ClibTimeArg, ClibCapacitanceArg


class ClibTimingGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibTiming',
                         pyname='ClibTiming',
                         namespace='YM',
                         header_include_files=['ym/ClibTiming.h'],
                         source_include_files=['pym/PyClibTiming.h',
                                               'pym/PyClibTime.h',
                                               'pym/PyClibCapacitance.h',
                                               'pym/PyClibResistance.h',
                                               'pym/PyClibTimingType.h',
                                               'pym/PyClibLut.h',
                                               'pym/PyExpr.h'])

        self.add_dealloc('default')

        def get_type(writer):
            writer.gen_return_pyobject('PyClibTimingType',
                                       'val.type()')
        self.add_getter('get_type',
                        func_body=get_type)
        self.add_attr('type',
                      getter_name='get_type',
                      doc_str='type(TimingType)')

        def get_timing_cond(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'val.timing_cond()')
        self.add_getter('get_timing_cond',
                        func_body=get_timing_cond)
        self.add_attr('timing_cond',
                      getter_name='get_timing_cond',
                      doc_str='timing condition expression')

        def meth_calc_rise_delay(writer):
            writer.gen_return_pyobject('PyClibTime',
                                       'val.calc_rise_delay(input_transition, output_capacitance)')
        self.add_method('calc_rise_delay',
                        func_body=meth_calc_rise_delay,
                        arg_list=[ClibTimeArg(name='input_transition',
                                              cvarname='input_transition'),
                                  ClibCapacitanceArg(name='output_capacitance',
                                                     cvarname='output_capacitance')],
                        doc_str='calculate RISE delay')

        def meth_calc_fall_delay(writer):
            writer.gen_return_pyobject('PyClibTime',
                                       'val.calc_fall_delay(input_transition, output_capacitance)')
        self.add_method('calc_fall_delay',
                        func_body=meth_calc_fall_delay,
                        arg_list=[ClibTimeArg(name='input_transition',
                                              cvarname='input_transition'),
                                  ClibCapacitanceArg(name='output_capacitance',
                                                     cvarname='output_capacitance')],
                        doc_str='calculate FALL delay')

        def meth_calc_rise_transition(writer):
            writer.gen_return_pyobject('PyClibTime',
                                       'val.calc_rise_transition(input_transition, output_capacitance)')
        self.add_method('calc_rise_transition',
                        func_body=meth_calc_rise_transition,
                        arg_list=[ClibTimeArg(name='input_transition',
                                              cvarname='input_transition'),
                                  ClibCapacitanceArg(name='output_capacitance',
                                                     cvarname='output_capacitance')],
                        doc_str='calculate RISE transition')

        def meth_calc_fall_transition(writer):
            writer.gen_return_pyobject('PyClibTime',
                                       'val.calc_fall_transition(input_transition, output_capacitance)')
        self.add_method('calc_fall_transition',
                        func_body=meth_calc_fall_transition,
                        arg_list=[ClibTimeArg(name='input_transition',
                                              cvarname='input_transition'),
                                  ClibCapacitanceArg(name='output_capacitance',
                                                     cvarname='output_capacitance')],
                        doc_str='calculate FALL transition')

        def get_intrinsic_rise(writer):
            writer.gen_return_pyobject('PyClibTime',
                                       'val.intrinsic_rise()')
        self.add_getter('get_intrinsic_rise',
                        func_body=get_intrinsic_rise)
        self.add_attr('intrinsic_rise',
                      getter_name='get_intrinsic_rise',
                      doc_str='intrinsic rise time')

        def get_intrinsic_fall(writer):
            writer.gen_return_pyobject('PyClibTime',
                                       'val.intrinsic_fall()')
        self.add_getter('get_intrinsic_fall',
                        func_body=get_intrinsic_fall)
        self.add_attr('intrinsic_fall',
                      getter_name='get_intrinsic_fall',
                      doc_str='intrinsic fall time')

        def get_slope_rise(writer):
            writer.gen_return_pyobject('PyClibTime',
                                       'val.slope_rise()')
        self.add_getter('get_slope_rise',
                        func_body=get_slope_rise)
        self.add_attr('slope_rise',
                      getter_name='get_slope_rise',
                      doc_str='slope rise time')

        def get_slope_fall(writer):
            writer.gen_return_pyobject('PyClibTime',
                                       'val.slope_fall()')
        self.add_getter('get_slope_fall',
                        func_body=get_slope_fall)
        self.add_attr('slope_fall',
                      getter_name='get_slope_fall',
                      doc_str='slpe fall time')

        def get_rise_resistance(writer):
            writer.gen_return_pyobject('PyClibResistance',
                                       'val.rise_resistance()')
        self.add_getter('get_rise_resistance',
                        func_body=get_rise_resistance)
        self.add_attr('rise_resistance',
                      getter_name='get_rise_resistance',
                      doc_str='rise resistance')

        def get_fall_resistance(writer):
            writer.gen_return_pyobject('PyClibResistance',
                                       'val.fall_resistance()')
        self.add_getter('get_fall_resistance',
                        func_body=get_fall_resistance)
        self.add_attr('fall_resistance',
                      getter_name='get_fall_resistance',
                      doc_str='fall resistance')

        def meth_rise_pin_resistance(writer):
            writer.gen_return_pyobject('PyClibResistance',
                                       'val.rise_pin_resistance(piece_id)')
        self.add_method('rise_pin_resistance',
                        func_body=meth_rise_pin_resistance,
                        arg_list=[UlongArg(name='piece_id',
                                           cvarname='piece_id')],
                        doc_str='return rise pin resistance')

        def meth_fall_pin_resistance(writer):
            writer.gen_return_pyobject('PyClibResistance',
                                       'val.fall_pin_resistance(piece_id)')
        self.add_method('fall_pin_resistance',
                        func_body=meth_fall_pin_resistance,
                        arg_list=[UlongArg(name='piece_id',
                                           cvarname='piece_id')],
                        doc_str='return fall pin resistance')

        def meth_rise_delay_intercept(writer):
            writer.gen_return_pyobject('PyClibTime',
                                       'val.rise_delay_intercept(piece_id)')
        self.add_method('rise_delay_intercept',
                        func_body=meth_rise_delay_intercept,
                        arg_list=[UlongArg(name='piece_id',
                                           cvarname='piece_id')],
                        doc_str='return rise delay intercept')

        def meth_fall_delay_intercept(writer):
            writer.gen_return_pyobject('PyClibTime',
                                       'val.fall_delay_intercept(piece_id)')
        self.add_method('fall_delay_intercept',
                        func_body=meth_fall_delay_intercept,
                        arg_list=[UlongArg(name='piece_id',
                                           cvarname='piece_id')],
                        doc_str='return fall delay intercept')

        def get_rise_transition(writer):
            writer.gen_return_pyobject('PyClibLut',
                                       'val.rise_transition()')
        self.add_getter('get_rise_transition',
                        func_body=get_rise_transition)
        self.add_attr('rise_transition',
                      getter_name='get_rise_transition',
                      doc_str='rise transition')

        def get_fall_transition(writer):
            writer.gen_return_pyobject('PyClibLut',
                                       'val.fall_transition()')
        self.add_getter('get_fall_transition',
                        func_body=get_fall_transition)
        self.add_attr('fall_transition',
                      getter_name='get_fall_transition',
                      doc_str='fall transition')

        def get_rise_propagation(writer):
            writer.gen_return_pyobject('PyClibLut',
                                       'val.rise_propagation()')
        self.add_getter('get_rise_propagation',
                        func_body=get_rise_propagation)
        self.add_attr('rise_propagation',
                      getter_name='get_rise_propagation',
                      doc_str='rise propagation')

        def get_fall_propagation(writer):
            writer.gen_return_pyobject('PyClibLut',
                                       'val.fall_propagation()')
        self.add_getter('get_fall_propagation',
                        func_body=get_fall_propagation)
        self.add_attr('fall_propagation',
                      getter_name='get_fall_propagation',
                      doc_str='fall propagation')

        def get_cell_rise(writer):
            writer.gen_return_pyobject('PyClibLut',
                                       'val.cell_rise()')
        self.add_getter('get_cell_rise',
                        func_body=get_cell_rise)
        self.add_attr('cell_rise',
                      getter_name='get_cell_rise',
                      doc_str='cell rise')

        def get_cell_fall(writer):
            writer.gen_return_pyobject('PyClibLut',
                                       'val.cell_fall()')
        self.add_getter('get_cell_fall',
                        func_body=get_cell_fall)
        self.add_attr('cell_fall',
                      getter_name='get_cell_fall',
                      doc_str='cell fall')

        self.add_conv('default')
        self.add_deconv('default')
