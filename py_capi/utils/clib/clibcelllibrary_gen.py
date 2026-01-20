#! /usr/bin/env python3

""" PyClibCellLibrary を生成するスクリプト

:file: clibcelllibrary_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import LongArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg
from .clib_arg import ClibSeqAttrArg


class ClibCellLibraryGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibCellLibrary',
                         pyname='ClibCellLibrary',
                         namespace='YM',
                         header_include_files=['ym/ClibCellLibrary.h'],
                         source_include_files=['pym/PyClibCellLibrary.h',
                                               'pym/PyClibCellClass.h',
                                               'pym/PyClibCellGroup.h',
                                               'pym/PyClibCell.h',
                                               'pym/PyClibCellType.h',
                                               'pym/PyClibIOMap.h',
                                               'pym/PyClibSeqAttr.h',
                                               'pym/PyClibPatGraph.h',
                                               'pym/PyClibPatType.h',
                                               'pym/PyClibTechnology.h',
                                               'pym/PyClibDelayModel.h',
                                               'pym/PyClibVarType.h',
                                               'pym/PyClibList.h',
                                               'pym/pyfstream.h',
                                               'pym/PyUlong.h',
                                               'pym/PyFloat.h',
                                               'pym/PyString.h'])

        self.add_dealloc('default')

        def meth_read_mislib(writer):
            writer.gen_return_pyobject('PyClibCellLibrary',
                                       'ClibCellLibrary::read_mislib(filename)')
        self.add_static_method('read_mislib',
                               func_body=meth_read_mislib,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               doc_str='read mislig(genlib)')

        def meth_read_liberty(writer):
            writer.gen_return_pyobject('PyClibCellLibrary',
                                       'ClibCellLibrary::read_liberty(filename)')
        self.add_static_method('read_liberty',
                               func_body=meth_read_liberty,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               doc_str='read liberty(.lib)')

        def meth_is_valid(writer):
            writer.gen_return_py_bool('val.is_valid()')
        self.add_method('is_valid',
                        func_body=meth_is_valid,
                        doc_str='True if valid')

        def get_name(writer):
            writer.gen_return_py_string('val.name()')
        self.add_getter('get_name',
                        func_body=get_name)
        self.add_attr('name',
                      getter_name='get_name',
                      doc_str='name')

        def get_technology(writer):
            writer.gen_return_pyobject('PyClibTechnology', 'val.technology()')
        self.add_getter('get_technology',
                        func_body=get_technology)
        self.add_attr('technology',
                      getter_name='get_technology',
                      doc_str='technology(cmos, fpga)')

        def get_delay_model(writer):
            writer.gen_return_pyobject('PyClibDelayModel', 'val.delay_model()')
        self.add_getter('get_delay_model',
                        func_body=get_delay_model)
        self.add_attr('delay_model',
                      getter_name='get_delay_model',
                      doc_str='delay model(generic_cmos, table_lookup, piecewise_cmos, cmos2, dcm, polynomial)')

        def get_piece_type(writer):
            writer.gen_return_pyobject('PyClibVarType', 'val.piece_type()')
        self.add_getter('get_piece_type',
                        func_body=get_piece_type)
        self.add_attr('piece_type',
                      getter_name='get_piece_type',
                      doc_str='ClibVarType for pice_define')

        def meth_piece_define(writer):
            writer.gen_return_pyobject('PyList<double, PyFloat>',
                                       'val.piece_define()')
        self.add_method('meth_piece_define',
                        func_body=meth_piece_define,
                        doc_str='return list of piece-defines')

        def get_bus_naming_style(writer):
            writer.gen_return_py_string('val.bus_naming_style()')
        self.add_getter('get_bus_naming_style',
                        func_body=get_bus_naming_style)
        self.add_attr('bus_naming_style',
                      getter_name='get_bus_naming_style',
                      doc_str='bus naming style')

        def get_date(writer):
            writer.gen_return_py_string('val.date()')
        self.add_getter('get_date',
                        func_body=get_date)
        self.add_attr('date',
                      getter_name='get_date',
                      doc_str='date')

        def get_revision(writer):
            writer.gen_return_py_string('val.revision()')
        self.add_getter('get_revision',
                        func_body=get_revision)
        self.add_attr('revision',
                      getter_name='get_revision',
                      doc_str='revision')

        def get_comment(writer):
            writer.gen_return_py_string('val.comment()')
        self.add_getter('get_comment',
                        func_body=get_comment)
        self.add_attr('comment',
                      getter_name='get_comment',
                      doc_str='comment')

        def get_time_unit(writer):
            writer.gen_return_py_string('val.time_unit().val()')
        self.add_getter('get_time_unit',
                        func_body=get_time_unit)
        self.add_attr('time_unit',
                      getter_name='get_time_unit',
                      doc_str='time unit')

        def get_voltage_unit(writer):
            writer.gen_return_py_string('val.voltage_unit().val()')
        self.add_getter('get_voltage_unit',
                        func_body=get_voltage_unit)
        self.add_attr('voltage_unit',
                      getter_name='get_voltage_unit',
                      doc_str='valtage unit')

        def get_current_unit(writer):
            writer.gen_return_py_string('val.current_unit().val()')
        self.add_getter('get_current_unit',
                        func_body=get_current_unit)
        self.add_attr('current_unit',
                      getter_name='get_current_unit',
                      doc_str='current unit')

        def get_pulling_resistance_unit(writer):
            writer.gen_return_py_string('val.pulling_resistance_unit().val()')
        self.add_getter('get_pulling_resistance_unit',
                        func_body=get_pulling_resistance_unit)
        self.add_attr('pulling_resistance_unit',
                      getter_name='get_pulling_resistance_unit',
                      doc_str='pulling resistance unit')

        def get_capacitive_load_unit(writer):
            writer.gen_auto_assign('val1', 'PyFloat::ToPyObject(val.capacitive_load_unit().float_val())')
            writer.gen_auto_assign('val2', 'PyString::ToPyObject(val.capacitive_load_unit().str_val())')
            writer.gen_return_buildvalue('(OO)', ['val1', 'val2'])
        self.add_getter('get_capacitive_load_unit',
                        func_body=get_capacitive_load_unit)
        self.add_attr('capacitive_load_unit',
                      getter_name='get_capacitive_load_unit',
                      doc_str='capacitive load unit')

        def get_leakage_power_unit(writer):
            writer.gen_return_py_string('val.leakage_power_unit().val()')
        self.add_getter('get_leakage_power_unit',
                        func_body=get_leakage_power_unit)
        self.add_attr('leakage_power_unit',
                      getter_name='get_leakage_power_unit',
                      doc_str='leakage power unit')

        def get_cell_num(writer):
            writer.gen_return_py_ulong('val.cell_num()')
        self.add_getter('get_cell_num',
                        func_body=get_cell_num)
        self.add_attr('cell_num',
                      getter_name='get_cell_num',
                      doc_str='number of cells')

        def meth_cell(writer):
            with writer.gen_if_block('PyUlong::Check(arg_obj)'):
                writer.gen_auto_assign('cell_id', 'PyUlong::Get(arg_obj)')
                writer.gen_return_pyobject('PyClibCell', 'val.cell(cell_id)')
            with writer.gen_elseif_block('PyString::Check(arg_obj)'):
                writer.gen_auto_assign('name', 'PyString::Get(arg_obj)')
                writer.gen_return_pyobject('PyClibCell', 'val.cell(name)')
            with writer.gen_else_block():
                writer.gen_type_error('"1st argument should be integer or string"')
        self.add_method('cell',
                        func_body=meth_cell,
                        arg_list=[RawObjArg(cvarname='arg_obj')],
                        doc_str='return cell')

        def meth_cell_list(writer):
            writer.gen_return_pyobject('PyClibCellList',
                                       'val.cell_list()')
        self.add_method('meth_cell_list',
                        func_body=meth_cell_list,
                        doc_str='return list of cells')

        def get_cell_group_num(writer):
            writer.gen_return_py_ulong('val.cell_group_num()')
        self.add_getter('get_cell_group_num',
                        func_body=get_cell_group_num)
        self.add_attr('cell_group_num',
                      getter_name='get_cell_group_num',
                      doc_str='number of cell groups')

        def meth_cell_group(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.cell_group(pos)')
        self.add_method('cell_group',
                        func_body=meth_cell_group,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return cell group')

        def meth_cell_group_list(writer):
            writer.gen_return_pyobject('PyClibCellGroupList',
                                       'val.cell_group_list()')
        self.add_method('meth_cell_group_list',
                        func_body=meth_cell_group_list,
                        doc_str='return list of cell groups')

        def get_npn_class_num(writer):
            writer.gen_return_py_ulong('val.npn_class_num()')
        self.add_getter('get_npn_class_num',
                        func_body=get_npn_class_num)
        self.add_attr('npn_class_num',
                      getter_name='get_npn_class_num',
                      doc_str='number of cell classes')

        def meth_npn_class(writer):
            writer.gen_return_pyobject('PyClibCellClass',
                                       'val.npn_class(pos)')
        self.add_method('npn_class',
                        func_body=meth_npn_class,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return cell class')

        def meth_npn_class_list(writer):
            writer.gen_return_pyobject('PyClibCellClassList',
                                       'val.npn_class_list()')
        self.add_method('meth_npn_class_list',
                        func_body=meth_npn_class_list,
                        doc_str='return list of cell classes')

        def get_const0_func(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.const0_func()')
        self.add_getter('get_const0_func',
                        func_body=get_const0_func)
        self.add_attr('const0_func',
                      getter_name='get_const0_func',
                      doc_str='Const0 cell group')

        def get_const1_func(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.const1_func()')
        self.add_getter('get_const1_func',
                        func_body=get_const1_func)
        self.add_attr('const1_func',
                      getter_name='get_const1_func',
                      doc_str='Const1 cell group')

        def get_buf_func(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.buf_func()')
        self.add_getter('get_buf_func',
                        func_body=get_buf_func)
        self.add_attr('buf_func',
                      getter_name='get_buf_func',
                      doc_str='Buffer cell group')

        def get_inv_func(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.inv_func()')
        self.add_getter('get_inv_func',
                        func_body=get_inv_func)
        self.add_attr('inv_func',
                      getter_name='get_inv_func',
                      doc_str='Inveter cell gropu')

        def meth_and_func(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.and_func(ni)')
        self.add_method('and_func',
                        func_body=meth_and_func,
                        arg_list=[UlongArg(name='ni',
                                           cvarname='ni')],
                        doc_str='get AND function cell group')

        def meth_nand_func(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.nand_func(ni)')
        self.add_method('nand_func',
                        func_body=meth_nand_func,
                        arg_list=[UlongArg(name='ni',
                                           cvarname='ni')],
                        doc_str='get NAND function cell group')

        def meth_or_func(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.or_func(ni)')
        self.add_method('or_func',
                        func_body=meth_or_func,
                        arg_list=[UlongArg(name='ni',
                                           cvarname='ni')],
                        doc_str='get OR function cell group')

        def meth_nor_func(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.nor_func(ni)')
        self.add_method('nor_func',
                        func_body=meth_nor_func,
                        arg_list=[UlongArg(name='ni',
                                           cvarname='ni')],
                        doc_str='get NOR function cell group')

        def meth_xor_func(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.xor_func(ni)')
        self.add_method('xor_func',
                        func_body=meth_xor_func,
                        arg_list=[UlongArg(name='ni',
                                           cvarname='ni')],
                        doc_str='get XOR function cell group')

        def meth_xnor_func(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.xnor_func(ni)')
        self.add_method('xnor_func',
                        func_body=meth_xnor_func,
                        arg_list=[UlongArg(name='ni',
                                           cvarname='ni')],
                        doc_str='get XNOR function cell group')

        def get_mux2_func(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.mux2_func()')
        self.add_getter('get_mux2_func',
                        func_body=get_mux2_func)
        self.add_attr('mux2_func',
                      getter_name='get_mux2_func',
                      doc_str='MUX2 function cell group')

        def get_mux4_func(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.mux4_func()')
        self.add_getter('get_mux4_func',
                        func_body=get_mux4_func)
        self.add_attr('mux4_func',
                      getter_name='get_mux4_func',
                      doc_str='MUX4 function cell group')

        def meth_find_ff_class(writer):
            writer.gen_return_pyobject('PyClibCellClassList',
                                       'val.find_ff_class(attr)')
        self.add_method('find_ff_class',
                        func_body=meth_find_ff_class,
                        arg_list=[ClibSeqAttrArg(name='attr',
                                                 cvarname='attr')],
                        doc_str='find FF cell class')

        def meth_find_latch_class(writer):
            writer.gen_return_pyobject('PyClibCellClassList',
                                       'val.find_latch_class(attr)')
        self.add_method('find_latch_class',
                        func_body=meth_find_latch_class,
                        arg_list=[ClibSeqAttrArg(name='attr',
                                                 cvarname='attr')],
                        doc_str='find LATCH cell class')

        def get_pg_pat_num(writer):
            writer.gen_return_py_ulong('val.pg_pat_num()')
        self.add_getter('get_pg_pat_num',
                        func_body=get_pg_pat_num)
        self.add_attr('pg_pat_num',
                      getter_name='get_pg_pat_num',
                      doc_str='number of PatGraphs')

        def meth_pg_pat(writer):
            writer.gen_return_pyobject('PyClibPatGraph', 'val.pg_pat(id)')
        self.add_method('pg_pat',
                        func_body=meth_pg_pat,
                        arg_list=[UlongArg(name='id',
                                           cvarname='id')],
                        doc_str='return PatGraph')

        def get_pg_max_input(writer):
            writer.gen_return_py_ulong('val.pg_max_input()')
        self.add_getter('get_pg_max_input',
                        func_body=get_pg_max_input)
        self.add_attr('pg_max_input',
                      getter_name='get_pg_max_input',
                      doc_str='max number of PatGraph inputs')

        def get_pg_node_num(writer):
            writer.gen_return_py_ulong('val.pg_node_num()')
        self.add_getter('get_pg_node_num',
                        func_body=get_pg_node_num)
        self.add_attr('pg_node_num',
                      getter_name='get_pg_node_num',
                      doc_str='number PatGraph nodes')

        def meth_pg_node_type(writer):
            writer.gen_return_pyobject('PyClibPatType',
                                       'val.pg_node_type(id)')
        self.add_method('pg_node_type',
                        func_body=meth_pg_node_type,
                        arg_list=[UlongArg(name='id',
                                           cvarname='id')],
                        doc_str='return Node Type')

        def meth_pg_input_id(writer):
            writer.gen_return_py_ulong('val.pg_input_id(id)')
        self.add_method('pg_input_id',
                        func_body=meth_pg_input_id,
                        arg_list=[UlongArg(name='id',
                                           cvarname='id')],
                        doc_str='return Input ID of the specified node')

        def meth_pg_input_node(writer):
            writer.gen_return_py_ulong('val.pg_input_node(input_id)')
        self.add_method('pg_input_node',
                        func_body=meth_pg_input_node,
                        arg_list=[UlongArg(name='input_id',
                                           cvarname='input_id')],
                        doc_str='return Node ID of the specified input')

        def get_pg_edge_num(writer):
            writer.gen_return_py_ulong('val.pg_edge_num()')
        self.add_getter('get_pg_edge_num',
                        func_body=get_pg_edge_num)
        self.add_attr('pg_edge_num',
                      getter_name='get_pg_edge_num',
                      doc_str='number of PatGraph edges')

        def meth_pg_edge(writer):
            writer.gen_auto_assign('val1', 'val.pg_edge_from(id)')
            writer.gen_auto_assign('val2', 'val.pg_edge_to(id)')
            writer.gen_auto_assign('val3', 'val.pg_edge_pos(id)')
            writer.gen_auto_assign('val4', 'val.pg_edge_inv(id)')
            writer.gen_return_buildvalue('(OOOO)', ['val1', 'val2', 'val3', 'val4'])
        self.add_method('pg_edge',
                        func_body=meth_pg_edge,
                        arg_list=[UlongArg(name='id',
                                           cvarname='id')],
                        doc_str='return Edge information(souce-id, sink-id, fanin-pos, inv)')

        def meth_dump(writer):
            writer.gen_stmt('val.dump(filename)')
            writer.gen_return_py_none()
        self.add_method('dump',
                        func_body=meth_dump,
                        arg_list=[StringArg(name='filename',
                                            cvarname='filename')],
                        doc_str='dump')

        def meth_restore(writer):
            writer.gen_return_pyobject('PyClibCellLibrary',
                                       'ClibCellLibrary::restore(filename)')
        self.add_static_method('restore',
                               func_body=meth_restore,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               doc_str='restore')

        def meth_display(writer):
            writer.gen_auto_assign('s', 'opyfstream(fout_obj)')
            writer.gen_stmt('val.display(s)')
            writer.gen_return_py_none()
        self.add_method('display',
                        func_body=meth_display,
                        arg_list=[RawObjArg(name='fout',
                                            cvarname='fout_obj')],
                        doc_str='display contents')

        def meth_to_string(writer):
            writer.gen_return_py_string('val.to_string()')
        self.add_method('to_string',
                        func_body=meth_to_string)

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
