#! /usr/bin/env python3

""" clib の列挙型の Python クラスを生成するスクリプト

:file: clibenum_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import EnumGen, EnumInfo
from mk_py_capi import OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg


class ClibTechnologyGen(EnumGen):

    def __init__(self):
        super().__init__(classname='ClibTechnology',
                         pyname='ClibTechnology',
                         namespace='YM',
                         enum_list=[EnumInfo('ClibTechnology::cmos', 'cmos', 'cmos'),
                                    EnumInfo('ClibTechnology::fpga', 'fpga', 'fpga')],
                         none_value='ClibTechnology::none',
                         ignore_case=True,
                         header_include_files=['ym/clib.h'],
                         source_include_files=['pym/PyClibTechnology.h',
                                               'pym/PyString.h'])


class ClibDelayModelGen(EnumGen):

    def __init__(self):
        super().__init__(classname='ClibDelayModel',
                         pyname='ClibDelayModel',
                         namespace='YM',
                         enum_list=[EnumInfo('ClibDelayModel::generic_cmos', 'generic_cmos', 'generic_cmos'),
                                    EnumInfo('ClibDelayModel::table_lookup', 'table_lookup', 'table_lookup'),
                                    EnumInfo('ClibDelayModel::piecewise_cmos', 'piecewise_cmos', 'piecewise_cmos'),
                                    EnumInfo('ClibDelayModel::cmos2', 'cmos2', 'cmos2'),
                                    EnumInfo('ClibDelayModel::dcm', 'dcm', 'dcm'),
                                    EnumInfo('ClibDelayModel::polynomial', 'polynomial', 'polynomial')],
                         none_value='ClibDelayModel::none',
                         ignore_case=True,
                         header_include_files=['ym/clib.h'],
                         source_include_files=['pym/PyClibDelayModel.h',
                                               'pym/PyString.h'])


class ClibDirectionGen(EnumGen):

    def __init__(self):
        super().__init__(classname='ClibDirection',
                         pyname='ClibDirection',
                         namespace='YM',
                         enum_list=[EnumInfo('ClibDirection::input', 'input', 'input'),
                                    EnumInfo('ClibDirection::output', 'output', 'output'),
                                    EnumInfo('ClibDirection::inout', 'inout', 'inout'),
                                    EnumInfo('ClibDirection::internal', 'internal', 'internal')],
                         none_value='ClibDirection::none',
                         ignore_case=True,
                         header_include_files=['ym/clib.h'],
                         source_include_files=['pym/PyClibDirection.h',
                                               'pym/PyString.h'])


class ClibTimingSenseGen(EnumGen):

    def __init__(self):
        super().__init__(classname='ClibTimingSense',
                         pyname='ClibTimingSense',
                         namespace='YM',
                         enum_list=[EnumInfo('ClibTimingSense::positive_unate', 'positive_unate', 'positive_unate'),
                                    EnumInfo('ClibTimingSense::negative_unate', 'negative_unate', 'negative_unate'),
                                    EnumInfo('ClibTimingSense::non_unate', 'non_unate', 'non_unate')],
                         none_value='ClibTimingSense::none',
                         ignore_case=True,
                         header_include_files=['ym/clib.h'],
                         source_include_files=['pym/PyClibTimingSense.h',
                                               'pym/PyString.h'])


class ClibTimingTypeGen(EnumGen):

    def __init__(self):
        super().__init__(classname='ClibTimingType',
                         pyname='ClibTimingType',
                         namespace='YM',
                         enum_list=[EnumInfo('ClibTimingType::combinational', 'combinational', 'combinational'),
                                    EnumInfo('ClibTimingType::combinational_rise', 'combinational_rise', 'combinational_rise'),
                                    EnumInfo('ClibTimingType::combinational_fall', 'combinational_fall', 'combinational_fall'),
                                    EnumInfo('ClibTimingType::three_state_enable', 'three_state_enable', 'three_state_enable'),
                                    EnumInfo('ClibTimingType::three_state_disable', 'three_state_disable', 'three_state_disable'),
                                    EnumInfo('ClibTimingType::three_state_enable_rise', 'three_state_enable_rise', 'three_state_enable_rise'),
                                    EnumInfo('ClibTimingType::three_state_enable_fall', 'three_state_enable_fall', 'three_state_enable_fall'),
                                    EnumInfo('ClibTimingType::three_state_disable_rise', 'three_state_disable_rise', 'three_state_disable_rise'),
                                    EnumInfo('ClibTimingType::three_state_disable_fall', 'three_state_disable_fall', 'three_state_disable_fall'),
                                    EnumInfo('ClibTimingType::rising_edge', 'rising_edge', 'rising_edge'),
                                    EnumInfo('ClibTimingType::falling_edge', 'falling_edge', 'falling_edge'),
                                    EnumInfo('ClibTimingType::preset', 'preset', 'preset'),
                                    EnumInfo('ClibTimingType::clear', 'clear', 'clear'),
                                    EnumInfo('ClibTimingType::hold_rising', 'hold_rising', 'hold_rising'),
                                    EnumInfo('ClibTimingType::hold_falling', 'hold_falling', 'hold_falling'),
                                    EnumInfo('ClibTimingType::setup_rising', 'setup_rising', 'setup_rising'),
                                    EnumInfo('ClibTimingType::setup_falling', 'setup_falling', 'setup_falling'),
                                    EnumInfo('ClibTimingType::recovery_rising', 'recovery_rising', 'recovery_rising'),
                                    EnumInfo('ClibTimingType::recovery_falling', 'recovery_falling', 'recovery_falling'),
                                    EnumInfo('ClibTimingType::skew_rising', 'skew_rising', 'skew_rising'),
                                    EnumInfo('ClibTimingType::skew_falling', 'skew_falling', 'skew_falling'),
                                    EnumInfo('ClibTimingType::removal_rising', 'removal_rising', 'removal_rising'),
                                    EnumInfo('ClibTimingType::removal_falling', 'removal_falling', 'removal_falling'),
                                    EnumInfo('ClibTimingType::non_seq_setup_rising', 'non_seq_setup_rising', 'non_seq_setup_rising'),
                                    EnumInfo('ClibTimingType::non_seq_setup_falling', 'non_seq_setup_falling', 'non_seq_setup_falling'),
                                    EnumInfo('ClibTimingType::non_seq_hold_rising', 'non_seq_hold_rising', 'non_seq_hold_rising'),
                                    EnumInfo('ClibTimingType::non_seq_hold_falling', 'non_seq_hold_falling', 'non_seq_hold_falling'),
                                    EnumInfo('ClibTimingType::nochange_high_high', 'nochange_high_high', 'nochange_high_high'),
                                    EnumInfo('ClibTimingType::nochange_high_low', 'nochange_high_low', 'nochange_high_low'),
                                    EnumInfo('ClibTimingType::nochange_low_high', 'nochange_low_high', 'nochange_low_high'),
                                    EnumInfo('ClibTimingType::nochange_low_low', 'nochange_low_low', 'nochange_low_low')],
                         none_value='ClibTimingType::none',
                         ignore_case=True,
                         header_include_files=['ym/clib.h'],
                         source_include_files=['pym/PyClibTimingType.h',
                                               'pym/PyString.h'])


class ClibVarTypeGen(EnumGen):

    def __init__(self):
        super().__init__(classname='ClibVarType',
                         pyname='ClibVarType',
                         namespace='YM',
                         enum_list=[EnumInfo('ClibVarType::input_net_transition', 'input_net_transition', 'input_net_transition'),
                                    EnumInfo('ClibVarType::total_output_net_capacitance', 'total_output_net_capacitance', 'total_output_net_capacitance'),
                                    EnumInfo('ClibVarType::output_net_length', 'output_net_length', 'output_net_length'),
                                    EnumInfo('ClibVarType::output_net_wire_cap', 'output_net_wire_cap', 'output_net_wire_cap'),
                                    EnumInfo('ClibVarType::output_net_pin_cap', 'output_net_pin_cap', 'output_net_pin_cap'),
                                    EnumInfo('ClibVarType::equal_or_opposite_output_net_capacitance', 'equal_or_opposite_output_net_capacitance', 'equal_or_opposite_output_net_capacitance'),
                                    EnumInfo('ClibVarType::input_transition_time', 'input_transition_time', 'input_transition_time'),
                                    EnumInfo('ClibVarType::related_out_total_output_net_capacitance', 'related_out_total_output_net_capacitance', 'related_out_total_output_net_capacitance'),
                                    EnumInfo('ClibVarType::related_out_output_net_length', 'related_out_output_net_length', 'related_out_output_net_length'),
                                    EnumInfo('ClibVarType::related_out_output_net_wire_cap', 'related_out_output_net_wire_cap', 'related_out_output_net_wire_cap'),
                                    EnumInfo('ClibVarType::related_out_output_net_pin_cap', 'related_out_output_net_pin_cap', 'related_out_output_net_pin_cap'),
                                    EnumInfo('ClibVarType::constrained_pin_transition', 'constrained_pin_transition', 'constrained_pin_transition'),
                                    EnumInfo('ClibVarType::related_pin_transition', 'related_pin_transition', 'related_pin_transition')],
                         none_value='ClibVarType::none',
                         ignore_case=True,
                         header_include_files=['ym/clib.h'],
                         source_include_files=['pym/PyClibVarType.h',
                                               'pym/PyString.h'])


class ClibCellTypeGen(EnumGen):

    def __init__(self):
        super().__init__(classname='ClibCellType',
                         pyname='ClibCellType',
                         namespace='YM',
                         enum_list=[EnumInfo('ClibCellType::Logic', 'Logic', 'Logic'),
                                    EnumInfo('ClibCellType::FF', 'FF', 'FF'),
                                    EnumInfo('ClibCellType::Latch', 'Latch', 'Latch'),
                                    EnumInfo('ClibCellType::FSM', 'FSM', 'FSM')],
                         none_value='ClibCellType::none',
                         ignore_case=True,
                         header_include_files=['ym/clib.h'],
                         source_include_files=['pym/PyClibCellType.h',
                                               'pym/PyString.h'])


class ClibCPVGen(EnumGen):

    def __init__(self):
        super().__init__(classname='ClibCPV',
                         pyname='ClibCPV',
                         namespace='YM',
                         enum_list=[EnumInfo('ClibCPV::L', 'L', 'L'),
                                    EnumInfo('ClibCPV::H', 'H', 'H'),
                                    EnumInfo('ClibCPV::N', 'N', 'N'),
                                    EnumInfo('ClibCPV::T', 'T', 'T'),
                                    EnumInfo('ClibCPV::X', 'X', 'X')],
                         ignore_case=True,
                         header_include_files=['ym/clib.h'],
                         source_include_files=['pym/PyClibCPV.h',
                                               'pym/PyString.h'])


class ClibPatTypeGen(EnumGen):

    def __init__(self):
        super().__init__(classname='ClibPatType',
                         pyname='ClibPatType',
                         namespace='YM',
                         enum_list=[EnumInfo('ClibPatType::Input', 'Input', 'Input'),
                                    EnumInfo('ClibPatType::And', 'And', 'And'),
                                    EnumInfo('ClibPatType::Xor', 'Xor', 'Xor')],
                         ignore_case=True,
                         header_include_files=['ym/clib.h'],
                         source_include_files=['pym/PyClibPatType.h',
                                               'pym/PyString.h'])
