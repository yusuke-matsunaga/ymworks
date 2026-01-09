#! /usr/bin/env python3

"""
'cell' group statement のパーサー生成用のデータ

[参考文献] Library Compiler Reference Manual Chapter 2:
 cell and model Group Description and Syntax

:file: cell_data.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2021 Yusuke Matsunaga, All rights reserved.
"""

from handlerdata import HandlerData

hd = HandlerData()


simple_attrs = (
    ('area', 'float'),
    ('auxiliary_pad_cell', 'bool'),
    ('base_name', 'string'),
    ('bus_naming_style', 'string'),
    ('cell_footprint', 'string'),
    ('cell_leakage_power', 'float'),
    ('clock_gating_integrated_cell', 'string'),
    ('contention_condition', 'function'),
    ('dont_fault', 'string'),
    ('dont_touch', 'bool'),
    ('dont_use', 'bool'),
    ('driver_type', 'string'),
    ('edif_name', 'string'),
    ('em_temp_degradation_factor', 'float'),
    ('fpga_cell_type', 'string'),
    ('fpga_domain_style', 'string'),
    ('geometry_print', 'string'),
    ('handle_negative_constraint', 'bool'),
    ('interface_timing', 'bool'),
    ('io_type', 'string'),
    ('is_clock_gating_cell', 'bool'),
    ('is_isolation_cell', 'bool'),
    ('is_level_shifter', 'bool'),
    ('level_shifter_type', 'string'),
    ('map_only', 'bool'),
    ('pad_cell', 'bool'),
    ('pad_type', 'string'),
    ('power_cell_type', 'string'),
    ('power_gating_cell', 'string'),
    ('preferred', 'bool'),
    ('scaling_factors', 'string'),
    ('sensitization_master', 'string'),
    ('single_bit_degenerate', 'string'),
    ('slew_type', 'string'),
    ('switch_cell_type', 'string'),
    ('threshold_voltage_group', 'string'),
    ('timing_model_type', 'string'),
    ('use_for_size_only', 'bool'),
    ('vhdl_name', 'string'),
)

complex_attrs = (
    ('(input|output)_voltage_range', 'float_float'),
    ('pin_equal', 'string'),
    ('pin_name_map', 'string_list'),
    ('pin_opposite', 'string_string'),
    ('rail_connection', 'string_string'),
    ('resource_usage', 'string_string'),
)

groups = (
    ('bundle', 'bundle'),
    ('bus', 'bus'),
    ('dynamic_current', 'dynamic_current'),
    ('ff', 'ff'),
    ('ff_bank', 'ff_bank'),
    ('fpga_condition', 'fpga_condition'),
    ('functional_yield_metric', 'functional_yield_metric'),
    ('generated_clock', 'generated_clock'),
    ('intrinsic_parasitic', 'intrinsic_parasitic'),
    ('latch', 'latch'),
    ('latch_bank', 'latch_bank'),
    ('leakage_current', 'leakage_current'),
    ('leakage_power', 'leakage_power'),
    ('lut', 'lut'),
    ('mode_definition', 'mode_definition'),
    ('pg_pin', 'pg_pin'),
    ('pin', 'pin'),
    #('pin_group', 'pin_group'), # manual にはない
    ('routing_track', 'routing_track'),
    ('statetable', 'statetable'),
    ('test_cell', 'test_cell'),
    ('type', 'type')
)

hd.add_group_data('cell', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs,
                  groups=groups)


####################################################
# scaled_cell
####################################################

hd.add_group_data('scaled_cell', 'string_string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
#   model
####################################################
model_simple_attrs = simple_attrs + (('cell_name', 'string'),)

model_complex_attrs = complex_attrs + (('short', 'string'),)

hd.add_group_data('model', 'string',
                  simple_attrs=model_simple_attrs,
                  complex_attrs=model_complex_attrs)


####################################################
#   dynamic_current
####################################################
simple_attrs = (
    ('related_inputs', 'string'),
    ('related_outputs', 'string'),
    ('when', 'function'),
)

complex_attrs = (
    ('typical_capacitances', 'float_vector'),
)

groups = (
    ('switching_group', 'switching_group'),
)

hd.add_group_data('dynamic_current', 'empty',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs,
                  groups=groups)

simple_attrs = (
    ('(max|min)_input_switching_count', 'int'),
)

complex_attrs = (
    ('input_switching_condition', 'string'),
    ('output_switching_condition', 'string'),
)

groups = (
    ('pg_current', 'sg_pg_current'),
)

hd.add_group_data('switching_group', 'empty',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs,
                  groups=groups)

groups = (
    ('vector', 'sg_vector'),
)

hd.add_group_data('sg_pg_current', 'empty',
                  groups=groups)

simple_attrs = (
    ('index_output', 'string'),
    ('reference_time', 'float'),
)

complex_attrs = (
    ('index_(1|2|3|4)', 'float_vector'),
    ('values', 'float_vector'),
    )

hd.add_group_data('sg_vector', 'empty',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
#   ff/ff_bank
####################################################
simple_attrs = (
    ('clear', 'function'),
    ('clear_preset_var(1|2)', 'string'),
    ('clocked_on(|_also)', 'function'),
    ('next_state', 'function'),
    ('preset', 'function'),
)

hd.add_group_data('ff', 'string_string',
                  simple_attrs=simple_attrs)

hd.add_group_data('ff_bank', 'string_string_int',
                  simple_attrs=simple_attrs)


####################################################
# fpga_condition
####################################################

groups = (
    ('fpga_condition_value', 'fpga_condition_value'),
    )

hd.add_group_data('fpga_condition', 'string',
                  groups=groups)

simple_attrs = (
    ('fpga_arc_condition', 'bool'),
)

hd.add_group_data('fpga_condition_value', 'string',
                  simple_attrs=simple_attrs)


####################################################
#   functional_yield_metric
####################################################
groups = (
    ('average_number_of_faults', 'average_number_of_faults'),
)

hd.add_group_data('functional_yield_metric', 'empty',
                  groups=groups)

simple_attrs = (
    ('variable_(1|2)', 'string'),
)

complex_attrs = (
    ('values', 'values'),
)

hd.add_group_data('average_number_of_faults', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)

simple_attrs = (
    ('when', 'function'),
    ('value', 'float'),
)

groups = (
    ('pg_current', 'lc_pg_current'),
    ('gate_leakage', 'gate_leakage'),
)

hd.add_group_data('leakage_current', 'empty',
                  simple_attrs=simple_attrs,
                  groups=groups)

simple_attrs = (
    ('value', 'float'),
)

hd.add_group_data('lc_pg_current', 'empty',
                  simple_attrs=simple_attrs)

simple_attrs = (
    ('input_(high|low)_value', 'float'),
)

hd.add_group_data('gate_leakage', 'string',
                  simple_attrs=simple_attrs)

simple_attrs = (
    ('powerlevel', 'string'),
    ('related_pg_pin', 'string'),
    ('when', 'function'),
    ('value', 'float'),
)

hd.add_group_data('leakage_power', 'empty',
                  simple_attrs=simple_attrs)


####################################################
#   generated_clock
####################################################
simple_attrs = (
    ('clock_pin', 'string'),
    ('master_pin', 'string'),
    ('divided_by', 'int'),
    ('multipled_by', 'int'),
    ('invert', 'bool'),
    ('duty_cycle', 'float'),
)

complex_attrs = (
    ('edges', 'int_list'),
    ('shifts', 'int_list'),
)

hd.add_group_data('generated_clock', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
#   intrinsic_parasitic
####################################################
simple_attrs = (
    ('when', 'function'),
)

groups = (
    ('intrinsic_capacitance', 'intrinsic_capacitance'),
    ('intrinsic_resistance', 'intrinsic_resistance'),
    ('total_capacitance', 'total_capacitance'),
)

hd.add_group_data('intrinsic_parasitic', 'string',
                  simple_attrs=simple_attrs,
                  groups=groups)

simple_attrs = (
    ('value', 'float'),
)

hd.add_group_data('intrinsic_capacitance', 'string',
                  simple_attrs=simple_attrs)

simple_attrs = (
    ('related_output', 'string'),
    ('value', 'float'),
)

hd.add_group_data('intrinsic_resistance', 'string',
                  simple_attrs=simple_attrs)

simple_attrs = (
    ('value', 'float'),
)

hd.add_group_data('total_capacitance', 'string',
                  simple_attrs=simple_attrs)


####################################################
#   latch/latch_bank
####################################################
simple_attrs = (
    ('clear', 'function'),
    ('cleare_preset_var(1|2)', 'string'),
    ('data_in', 'function'),
    ('enable(|_also)', 'function'),
    ('preset', 'function'),
)

hd.add_group_data('latch', 'string_string',
                  simple_attrs=simple_attrs)

hd.add_group_data('latch_bank', 'string_string_int',
                  simple_attrs=simple_attrs)


####################################################
#   lut
####################################################
simple_attrs = (
    ('input_pins', 'string'),
)

hd.add_group_data('lut', 'string',
                  simple_attrs=simple_attrs)


####################################################
#   mode_definition
####################################################

groups = (
    ('mode_value', 'mode_value'),
)

hd.add_group_data('mode_definition', 'string',
                  groups=groups)

simple_attrs = (
    ('when', 'function'),
    ('sdf_cond', 'string'),
)

hd.add_group_data('mode_value', 'string',
                  simple_attrs=simple_attrs)


####################################################
#   pg_pin
####################################################
simple_attrs = (
    ('voltage_name', 'string'),
    ('pg_type', 'string'),
)

hd.add_group_data('pg_pin', 'string',
                  simple_attrs=simple_attrs)


####################################################
#   routing_track
####################################################

simple_attrs = (
    ('tracks', 'int'),
    ('total_track_area', 'float'),
)

complex_attrs = (
#    ('short', ), マニュアルに記述なし．
)

hd.add_group_data('routing_track', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
#   statetable
####################################################

simple_attrs = (
    ('table', 'string'),
)

hd.add_group_data('statetable', 'string_string',
                  simple_attrs=simple_attrs)


####################################################
#   test_cell
####################################################
groups = (
    ('ff', 'ff'),
    ('ff_bank', 'ff_bank'),
    ('latch', 'latch'),
    ('latch_bank', 'latch_bank'),
    ('pin', 'pin'),
    ('state_table', 'statetable'),
)

hd.add_group_data('test_cell', 'empty',
                  groups=groups)
