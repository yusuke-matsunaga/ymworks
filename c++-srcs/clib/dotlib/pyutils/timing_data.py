#! /usr/bin/env python3

"""
'timing' group statement のパーサー生成用のデータ

[参考文献] Library Compiler Reference Manual, Caphter 3:
pin Group Description and Syntax
3-115 'timing Group in a pin Group'

:file: timing_data.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2021 Yusuke Matsunaga, All rights reserved.
"""

from handlerdata import HandlerData

hd = HandlerData()


####################################################
# timing
####################################################

simple_attrs = (
    ('clock_gating_flag', 'bool'),
    ('default_timing', 'bool'),
    ('(fall|rise)_resistance', 'float'),
    ('fpga_arc_condition', 'function'),
    ('fpga_domain_style', 'string'),
    ('interdependence_id', 'int'),
    ('intrinsic_(fall|rise)', 'float'),
    ('related_bus_equivalent', 'string'),
    ('related_bus_pins', 'string'),
    ('related_output_pin', 'string'),
    ('related_pin', 'string'),
    ('sdf_cond(|_end|_start)', 'function'),
    ('sdf_edges', 'string'),
    ('slope_(fall|rise)', 'float'),
    ('steady_state_resistance_(above_high|below_low|high|low)', 'float'),
    ('tied_off', 'bool'),
    ('timing_sense', 'timing_sense'),
    ('timing_type', 'timing_type'),
    ('wave_(fall|rise)_sampling_index', 'int'),
    ('when(|_end|_start)', 'function')
)

complex_attrs = (
    ('(fall|rise)_delay_intercept', 'int_float'),
    ('(fall|rise)_pin_resistance', 'int_float'),
    ('mode', 'string_string'),
    ('pin_name_map', 'string_list'),
    ('wave_(fall|rise)', 'int_vector'),
    ('wave_(fall|rise)_time_interval', 'float_vector'),
)

groups = (
    ('cell_degradation', 'cell_degradation'),
    ('cell_(fall|rise)', 'cell_rf'),
    ('compact_ccs_(fall|rise)', 'compact_ccs'),  # マニュアルに記載なし
    ('(fall|rise)_(constraint|propagation|transition)', 'table'),
    ('noise_immunity_(above_high|below_low|high|low)', 'noise_immunity'),
    ('output_current_(fall|rise)', 'output_current_rf'),
    ('propagated_noise_(height|peak_time_ratio|width)_(above_high|below_low|high|low)',
     'propagated_noise_height'),
    ('receiver_capacitance(1|2)_(fall|rise)', 'receiver_capacitance_value'),
    ('retaining_(fall|rise)', 'table'),
    ('retain_(fall|rise)_slew', 'table'),
    ('steady_state_current_(high|low|tristate)', 'table'),
)

hd.add_group_data('timing', 'opt_string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs,
                  groups=groups)


####################################################
# cell_degradation
####################################################
complex_attrs = (
    ('index_1', 'float_vector'),
    ('coefs', 'float_vector'),
    ('orders', 'int_vector'),
    ('values', 'values'),
    ('variable_n_range', 'float_vector')
)

groups = (
    ('domain', 'domain'),
)

hd.add_group_data('cell_degradation', 'string',
                  complex_attrs=complex_attrs,
                  groups=groups)


####################################################
# cell_rf
####################################################

complex_attrs = (
    ('index_(1|2|3)', 'float_vector'),
    ('values', 'values')
)

groups = (
    ('domain', 'domain'),
)

hd.add_group_data('cell_rf', 'string',
                  complex_attrs=complex_attrs,
                  groups=groups)


####################################################
# compact_ccs
####################################################

simple_attrs = (
    ('base_curves_group', 'string'),
)

complex_attrs = (
    ('values', 'values'),
)

hd.add_group_data('compact_ccs', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
# noise_immunity
####################################################

complex_attrs = (
    ('coefs', 'float_vector'),
    ('orders', 'int_vector'),
    ('values', 'values'),
)

groups = (
    ('domain', 'domain'),
)

hd.add_group_data('noise_immunity', 'string',
                  complex_attrs=complex_attrs,
                  groups=groups)


####################################################
# output_current_rf
####################################################

groups = (
    ('vector', 'oc_vector'),
)

hd.add_group_data('output_current_rf', 'string',
                  groups=groups)

simple_attrs = (
    ('reference_time', 'float'),
)

complex_attrs = (
    ('index_(1|2|3)', 'float_vector'),
    ('values', 'values'),
)

hd.add_group_data('oc_vector', 'string',
                  simple_attrs=simple_attrs,
                  complex_attrs=complex_attrs)


####################################################
# propagated_noise_height
####################################################

complex_attrs = (
    ('coefs', 'float_vector'),
    ('orders', 'int_vector'),
    ('valures', 'values')
)

groups = (
    ('domain', 'domain'),
)

hd.add_group_data('propagated_noise_height', 'string',
                  complex_attrs=complex_attrs,
                  groups=groups)

groups = (
    ('receiver_capacitance(1|2)_(fall|rise)', 'receiver_capacitance_value'),
)

hd.add_group_data('receiver_capacitance', 'empty',
                  groups=groups)

complex_attrs = (
    ('values', 'values'),
)

hd.add_group_data('receiver_capacitance_value', 'string',
                  complex_attrs=complex_attrs)


####################################################
# retaining_rf
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

hd.add_group_data('retaining_rf', 'string',
                  complex_attrs=complex_attrs,
                  groups=groups)
