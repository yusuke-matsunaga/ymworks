#! /usr/bin/env python3

"""
'library' group statement のパーサー生成用データ

[参考文献] Library Compiler Reference Manual, Chapter 1:
Technology Library Group Description and Syntax

:file: library_data.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2021 Yusuke Matsunaga, All rights reserved.
"""

from handlerdata import HandlerData

hd = HandlerData()


simple_attrs = (
    ('bus_naming_style', 'string'),
    ('comment', 'string'),
    ('current_unit', 'string'),
    ('date', 'string'),
    ('default_fpga_isd', 'string'),
    ('default_threshold_voltage_group', 'string'),
    ('delay_model', 'delay_model'),
    ('em_temp_degradation_factor', 'float'),
    ('fpga_domain_style', 'string'),
    ('fpga_technology', 'string'),
    ('(input|output)_threshold_pct_(fall|rise)', 'float'),
    ('in_place_swap_mode', 'string'),
    ('leakage_power_unit', 'string'),
    ('nom_calc_mode', 'string'),
    ('nom_(process|temperature|voltage)', 'float'),
    ('piece_type', 'string'),
    ('power_model', 'string'),
    ('power_unit', 'string'),
    ('preferred_(input|output)_pad_voltage', 'string'),
    ('preferred_output_pad_slew_rate_control', 'string'),
    ('pulling_resistance_unit', 'string'),
    ('resistance_unit', 'string'),
    ('revision', 'string'),
    ('simulation', 'bool'),
    ('slew_derate_from_library', 'float'),
    ('slew_(lower|upper)_threshold_pct_(fall|rise)', 'float'),
    ('threshold_voltage_group', 'string'),
    ('time_unit', 'string'),
    ('voltage_unit', 'string'),
    ('default_cell_leakage_power', 'float'),
    ('default_connection_class', 'string'),
    ('default_(fall|rise)_delay_intercept', 'float'),
    ('default_(fall|rise)_pin_resistance', 'float'),
    ('default_fanout_load', 'float'),
    ('default_(inout|input|output)_pin_cap', 'float'),
    ('default_(inout|input|output)_pin_(fall|rise)_res', 'float'),
    ('default_intrinsic_(fall|rise)', 'float'),
    ('default_leakage_power_density', 'float'),
    ('default_max_capacitance', 'float'),
    ('default_max_fanout', 'float'),
    ('default_max_transition', 'float'),
    ('default_max_utilization', 'float'),
    ('default_min_porosity', 'float'),
    ('default_operating_conditions', 'string'),
    ('default_slope_(fall|rise)', 'float'),
    ('default_wire_load', 'string'),
    ('default_wire_load_area', 'float'),
    ('default_wire_load_capacitance', 'float'),
    ('default_wire_load_mode', 'string'),
    ('default_wire_load_resistance', 'float'),
    ('default_wire_load_selection', 'string'),

    ('k_(process|temp|volt)_cell_(fall|rise)', 'float'),
    ('k_(process|temp|volt)_cell_leakage_power', 'float'),
    ('k_(process|temp|volt)_drive_current', 'float'),
    ('k_(process|temp|volt)_drive_(fall|rise)', 'float'),
    ('k_(process|temp|volt)_(fall|rise)_delay_intercept', 'float'),
    ('k_(process|temp|volt)_(fall|rise)_pin_resistance', 'float'),
    ('k_(process|temp|volt)_(fall|rise)_propagation', 'float'),
    ('k_(process|temp|volt)_(fall|rise)_transition', 'float'),
    ('k_(process|temp|volt)_hold_(fall|rise)', 'float'),
    ('k_(process|temp|volt)_internal_power', 'float'),
    ('k_(process|temp|volt)_intrinsic_(fall|rise)', 'float'),
    ('k_(process|temp|volt)_min_period', 'float'),
    ('k_(process|temp|volt)_min_pulse_width_(high|low)', 'float'),
    ('k_(process|temp|volt)_nochange_(fall|rise)', 'float'),
    ('k_(process|temp|volt)_pin_cap', 'float'),
    ('k_(process|temp|volt)_(recovery|removal|setup|skew|slope)_(fall|rise)', 'float'),
    ('k_(process|temp|volt)_wire_cap', 'float'),
    ('k_(process|temp|volt)_wire_res', 'float'),
)

complex_attrs = (
    ('capacitive_load_unit', 'float_string'),
    ('default_part', 'string_string'),
    ('define', 'string_string_string'),
    ('define_cell_area',  'string_string'),
    ('define_group', 'string_string'),
    ('library_features', 'string_list'),
    ('piece_define', 'string'),
    ('routing_layers', 'string_list'),
    ('technology', 'technology'),
    ('voltage_map', 'string_float')
)

groups = (
    ('base_curves', 'base_curves'),
    ('cell', 'cell'),
    ('compact_lut_template', 'clut_template'),
    ('dc_current_template', 'template'),
    ('em_lut_template', 'template'),
    ('(fall|rise)_net_delay', 'table'),
    ('(fall|rise)_transition_degradation', 'table'),
    ('faults_lut_template', 'template'),
    ('fpga_isd', 'fpga_isd'),
    ('input_voltage', 'input_voltage'),
    ('iv_lut_template', 'template'),
    ('lu_table_template', 'template'),
    ('maxcap_lut_template', 'template'),
    ('maxtrans_lut_template', 'template'),
    ('noise_lut_template', 'template'),
    ('output_current_template', 'template'),
    ('operating_conditions', 'operating_conditions'),
    ('output_voltage', 'output_voltage'),
    ('part', 'part'),
    ('poly_template', 'poly_template'),
    ('power_lut_template', 'template'),
    ('power_poly_template', 'poly_template'),
    ('power_supply', 'power_supply'),
    ('propagation_lut_template', 'template'),
    ('scaled_cell', 'scaled_cell'),
    #    ('scaling_factors', 'scaling_factors'), マニュアルに詳細な記述なし．
    ('sensitization', 'sensitization'),
    ('timing', 'timing'),
    ('timing_range', 'timing_range'),
    ('type', 'type'),
    ('wire_load', 'wire_load'),
    ('wire_load_selection', 'wire_load_selection'),
    ('wire_load_table', 'wire_load_table')
)

hd.add_group_data('library', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs,
                  groups=groups)


####################################################
# base_curves
####################################################

simple_attrs = (
    ('base_curve_type', 'string'),
)

complex_attrs = (
    ('curve_(x|y)', 'int_float_vector'),
)

hd.add_group_data('base_curves', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
# clut_template
####################################################

simple_attrs = (
    ('base_curves_group', 'string'),
    ('variable_(1|2|3)', 'vartype'),
)

complex_attrs = (
    ('index_(1|2|3)', 'float_vector'),
)

hd.add_group_data('clut_template', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
# domain
####################################################

simple_attrs = (
    ('calc_mode', 'string'),
)

complex_attrs = (
    ('coefs', 'float_vector'),
    ('orders', 'int_vector'),
    ('variable_(1|2|3)_range', 'float_float')
)

hd.add_group_data('domain', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
# fpga_isd
####################################################

simple_attrs = (
    ('drive', 'string'),
    ('io_type', 'string'),
    ('slew', 'string')
)

hd.add_group_data('fpga_isd', 'string',
                  simple_attrs=simple_attrs)


####################################################
# input_voltage
####################################################

simple_attrs = (
    ('vi(l|h|min|max)', 'expr'),
)

hd.add_group_data('input_voltage', 'string',
                  simple_attrs=simple_attrs)


####################################################
# output_attrs
####################################################

simple_attrs = (
    ('vo(l|h|min|max)', 'expr'),
)

hd.add_group_data('output_voltage', 'string',
                  simple_attrs=simple_attrs)


####################################################
# operation_condition
####################################################

simple_attrs = (
    ('calc_mode', 'string'),
    ('parametr(1|2|3|4|5)', 'float'),
    ('process', 'float'),
    ('temperature', 'float'),
    ('tree_type', 'string'),
    ('voltage', 'float')
)

complex_attrs = (
    ('power_rail', 'string_float'),
)

hd.add_group_data('operating_conditions', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
#  part
####################################################

simple_attrs = (
    ('default_step_level', 'string'),
    ('fpga_isd', 'string'),
    ('num_blockrams', 'int'),
    ('num_cols', 'int'),
    ('num_ffs', 'int'),
    ('num_luts', 'int'),
    ('num_rows', 'int'),
    ('pin_count', 'int')
)

complex_attrs = (
    ('max_count', 'string_int'),
    ('valid_speed_grade', 'string_list'),
    ('valid_step_levels', 'string_list'),
)

groups = (
    ('speed_grade', 'speed_grade'),
)

hd.add_group_data('part', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
# speed_grade
####################################################

simple_attrs = (
    ('fpga_isd', 'string'),
)

complex_attrs = (
    ('step_level', 'string'),
)

hd.add_group_data('speed_grade', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
# poly_template
####################################################
complex_attrs = (
    ('variables', 'vartype_list'),
    ('variable_(1|2|3)_range', 'float_float'),
    ('mapping', 'string_string'),
    ('orders', 'int_vector'),
)

groups = (
    ('domain', 'domain'),
)

hd.add_group_data('poly_template', 'string',
                  complex_attrs=complex_attrs,
                  groups=groups)


####################################################
# power_supply
####################################################

simple_attrs = (
    ('default_power_rail', 'string'),
)

complex_attrs = (
    ('powerrail', 'string_float'),
)

hd.add_group_data('power_supply', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
# sensitization
####################################################

complex_attrs = (
    ('pin_names', 'string_list'),
    ('vector', 'int_string'),
)

hd.add_group_data('sensitization', 'string',
                  complex_attrs=complex_attrs)


####################################################
# table
####################################################

complex_attrs = (
    ('index_(1|2|3)', 'float_vector'),
    ('values', 'values'),
    ('orders', 'int_vector'),
    ('coefs', 'float_vector'),
    ('variable_(1|2|3)_range', 'float_float')
)

groups = (
    ('domain', 'domain'),
)

hd.add_group_data('table', 'string',
                  complex_attrs=complex_attrs,
                  groups=groups)


####################################################
# template
####################################################

simple_attrs = (
    ('variable_(1|2|3)', 'vartype'),
)

complex_attrs = (
    ('index_(1|2|3)', 'float_vector'),
)

hd.add_group_data('template', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
# timing_range
####################################################

simple_attrs = (
    ('(faster|slower)_factor', 'float'),
)

hd.add_group_data('timing_range', 'string',
                  simple_attrs=simple_attrs)


####################################################
# type
####################################################

simple_attrs = (
    ('base_type', 'string'),
    ('bit_(from|to|width)', 'int'),
    ('data_type', 'string'),
    ('downto', 'bool'),
)

hd.add_group_data('type', 'string',
                  simple_attrs=simple_attrs)


####################################################
# wire_load
####################################################

simple_attrs = (
    ('area', 'float'),
    ('capacitance', 'float'),
    ('resistance', 'float'),
    ('slope', 'float')
)

complex_attrs = (
    ('fanout_length', 'fanout_length'),
)

hd.add_group_data('wire_load', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
# wire_load_from_area
####################################################

complex_attrs = (
    ('wire_load_from_area', 'float_float_string'),
)

hd.add_group_data('wire_load_selection', 'opt_string',
                  complex_attrs=complex_attrs)


####################################################
# wire_load_table
####################################################

complex_attrs = (
    ('fanout_(area|capacitance|length|resistance)', 'int_float'),
)

hd.add_group_data('wire_load_table', 'string',
                  complex_attrs=complex_attrs)
