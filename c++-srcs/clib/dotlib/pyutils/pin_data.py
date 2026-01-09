#! /usr/bin/env python3

"""
'pin' group statementt のパーサー生成用のデータ

[参考文献] Library Compiler Reference Manual, Caphter 3:
pin Group Description and Syntax
3-1 'pin Group'

:file: pin_data.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2021 Yusuke Matsunaga, All rights reserved.
"""

from handlerdata import HandlerData

hd = HandlerData()


####################################################
# pin
####################################################

simple_attrs = (
    ('bit_width', 'int'),
    ('capacitance', 'float'),
    ('clock', 'bool'),
    ('clock_gate_(clock|enable|test|obs|out)_pin', 'bool'),
    ('complementary_pin', 'string'),
    ('connection_class', 'string'),
    ('direction', 'direction'),
    ('dont_fault', 'string'),
    ('drive_current', 'float'),
    ('driver_type', 'string'),
    ('(fall|rise)_capacitance', 'float'),
    ('(fall|rise)_current_slope_(after|before)_threshold', 'float'),
    ('(fall|rise)_time_(after|before)_threshold', 'float'),
    ('fanout_load', 'float'),
    ('fault_model', 'string'),
    ('function', 'function'),
    ('has_builtin_pad', 'bool'),
    ('hysterisis', 'bool'),
    ('input_map', 'string'),
    ('input_signal_level', 'string'),
    ('input_threshold_pct_(fall|rise)', 'float'),
    ('input_voltage', 'string'),
    ('internal_node', 'string'),
    ('inverted_output', 'bool'),
    ('is_pad', 'bool'),
    ('isolation_cell_enable_pin', 'bool'),
    ('level_shifter_enable_pin', 'bool'),
    ('map_to_logic', 'expr'),
    ('(max|min)_capacitance', 'float'),
    ('(max|min)_fanout', 'float'),
    ('(max|min)_input_noise_width', 'float'),
    ('(max|min)_transition', 'float'),
    ('min_period', 'float'),
    ('min_pulse_width_(high|low)', 'float'),
    ('multicell_pad_pin', 'bool'),
    ('nextstate_type', 'string'),
    ('output_signal_level', 'string'),
    ('output_voltage', 'string'),
    ('pg_function', 'function'),
    ('pin_func_type', 'string'),
    ('power_down_function', 'function'),
    ('prefer_tied', 'function'),
    ('primary_output', 'bool'),
    ('pulling_current', 'float'),
    ('pulling_resistance', 'float'),
    ('pulse_clock', 'string'),
    ('related_ground_pin', 'string'),
    ('related_power_pin', 'string'),
    ('signal_type', 'string'),
    ('slew_control', 'string'),
    ('slew_(lower|upper)_threshold_pct_(fall|rise)', 'float'),
    ('state_function', 'function'),
    ('std_cell_main_rail', 'bool'),
    ('switch_function', 'function'),
    ('switch_pin', 'bool'),
    ('test_output_only', 'bool'),
    ('three_state', 'function'),
    ('vhdl_name', 'string'),
    ('x_function', 'function')
)

complex_attrs = (
    ('(fall|rise)_capacitance_range', 'float_float'),
    ('power_gating_pin', 'string_int')
)

groups = (
    ('ccsn_(first|last)_stage', 'ccsn'),
    ('dc_current', 'table'),
    ('electromigration', 'electromigration'),
    ('hyperbolic_noise_(above_high|below_low|high|low)', 'hyperbolic_noise'),
    #('(input|output)_signal_swing', 'signal_swing'),
    ('internal_power', 'internal_power'),
    ('max_cap', 'max_cap'),
    ('max_trans', 'table'),
    ('min_pulse_width', 'min_pulse_width'),
    ('minimum_period', 'minimum_period'),
    ('pin_capacitance', 'pin_capacitance'),
    ('timing', 'timing'),
    ('tlatch', 'tlatch'),
)

hd.add_group_data('pin', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs,
                  groups=groups)


####################################################
#  bundle
####################################################

bundle_complex_attrs = (
    ('members', 'string'),
)

hd.add_group_data('bundle', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=bundle_complex_attrs,
                  groups=groups)


####################################################
#  bus
####################################################

bus_simple_attrs = simple_attrs + (('bus_type', 'string'), )

hd.add_group_data('bus', 'string',
                  simple_attrs=bus_simple_attrs,
                  groups=groups)

####################################################
#  ccsn
####################################################

simple_attrs = (
    ('is_needge', 'bool'),
    ('is_inverting', 'bool'),
    ('stage_type', 'string'),
    ('miller_cap_(fall|rise)', 'float'),
    ('when', 'bool')
)

groups = (
    ('dc_current', 'ccsn_dc_current'),
    ('output_voltage_(rall|rise)', 'ccsn_output_voltage'),
    ('propagated_noise_(high|low)', 'ccsn_propagated_noise')
)

hd.add_group_data('ccsn', 'empty',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)

complex_attrs = (
    ('index_(1|2)', 'float_vector'),
    ('values', 'values')
)

hd.add_group_data('ccsn_dc_current', 'string',
                  complex_attrs=complex_attrs)

complex_attrs = (
    ('index_(1|2|3)', 'float_vector'),
    ('values', 'values')
)

hd.add_group_data('ccsn_output_voltage', 'string',
                  complex_attrs=complex_attrs)

complex_attrs = (
    ('index_(1|2|3|4)', 'float_vector'),
    ('values', 'values')
)

hd.add_group_data('ccsn_propagated_noise', 'string',
                  complex_attrs=complex_attrs)


####################################################
# electromigration
####################################################

simple_attrs = (
    ('related_pin', 'string'),
    ('related_bus_pins', 'string'),
    ('when', 'function')
)

complex_attrs = (
    ('index_(1|2)', 'float_vector'),
    ('values', 'values')
)

groups = (
    ('em_max_toggle_rate', 'em_max_toggle_rate'),
)

hd.add_group_data('electromigration', 'empty',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs,
                  groups=groups)

hd.add_group_data('em_max_toggle_rate', 'string')


####################################################
# hyperbolic_noise
####################################################

simple_attrs = (
    ('area_coefficent', 'float'),
    ('height_coefficient', 'float'),
    ('width_coefficient', 'float'),
)

hd.add_group_data('hyperbolic_noise', 'empty',
                  simple_attrs=simple_attrs)


####################################################
# internal_power
####################################################

simple_attrs = (
    ('equal_or_opposite_output', 'string'),
    ('(falling|rising)_together_group', 'string'),
    ('power_level', 'string'),
    ('related_pin', 'string'),
    ('switching_interval', 'float'),
    ('switching_together_group', 'string'),
    ('when', 'function'),
)

groups = (
    ('domain', 'domain'),
    ('(|fall_|rise_)power', 'power'),
)

hd.add_group_data('internal_power', 'empty',
                  simple_attrs=simple_attrs,
                  groups=groups)

complex_attrs = (
    ('index_(1|2|3)', 'float_vector'),
    ('values', 'values'),
    ('orders', 'int_vector'),
    ('coefs', 'float_vector')
)

groups = (
    ('domain', 'domain'),
)

hd.add_group_data('power', 'string',
                  complex_attrs=complex_attrs,
                  groups=groups)


####################################################
# max_cap
# マニュアルに記載なし
####################################################

hd.add_group_data('max_cap', 'string')


####################################################
# max_trans
####################################################

complex_attrs = (
    ('variable_(1|2|n)_range', 'float_vector'),
    ('orders', 'int_vector'),
    ('coefs', 'float_vector'),
)

hd.add_group_data('max_trans', 'string',
                  complex_attrs=complex_attrs)


####################################################
# min_pulse_width
####################################################

simple_attrs = (
    ('constraint_(high|low)', 'float'),
    ('when', 'function'),
    ('sdf_cond', 'function'),
)

hd.add_group_data('min_pulse_width', 'empty',
                  simple_attrs=simple_attrs)


####################################################
# minimum_period
####################################################

simple_attrs = (
    ('constraint', 'float'),
    ('when', 'function'),
    ('sdf_cond', 'function')
)

hd.add_group_data('minimum_period', 'empty',
                  simple_attrs=simple_attrs)


####################################################
# pin_capacitance
####################################################

groups = (
    ('capacitance', 'pc_capacitance'),
    ('(fall|rise)_capacitance', 'pc_capacitance'),
    ('(fall|rise)_capacitance_range', 'pc_capacitance_range')
)

hd.add_group_data('pin_capacitance', 'empty',
                  groups=groups)

complex_attrs = (
    ('orders', 'int_vector'),
    ('coefs', 'float_vector')
)

hd.add_group_data('pc_capacitance', 'string',
                  complex_attrs=complex_attrs)

groups = (
    ('(lower|upper)', 'pc_capacitance_range_lu'),
)

hd.add_group_data('pc_capacitance_range', 'string',
                  groups=groups)

complex_attrs = (
    ('variable_(1|2|n)_range', 'float_vector'),
    ('orders', 'int_vector'),
    ('coefs', 'float_vector')
)

hd.add_group_data('pc_capacitance_range_lu', 'string',
                  complex_attrs=complex_attrs)

####################################################
# tlatch
####################################################

simple_attrs = (
    ('edge_type', 'string'),
    ('tdisable', 'bool')
)

hd.add_group_data('tlatch', 'string',
                  simple_attrs=simple_attrs)
