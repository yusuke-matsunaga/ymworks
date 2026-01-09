#! /usr/bin/env python3

""" PyClibLut を生成するスクリプト

:file: cliblut_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import LongArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg


class ClibLutGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibLut',
                         pyname='ClibLut',
                         namespace='YM',
                         header_include_files=['ym/ClibLut.h'],
                         source_include_files=['pym/PyClibLut.h',
                                               'pym/PyClibVarType.h',
                                               'pym/PyList.h',
                                               'pym/PyFloat.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc('default')

        def get_dimension(writer):
            writer.gen_return_py_ulong('val.dimension()')
        self.add_getter('get_dimension',
                        func_body=get_dimension)
        self.add_attr('dimension',
                      getter_name='get_dimension',
                      doc_str='dimension')

        def meth_variable_type(writer):
            writer.gen_return_pyobject('PyClibVarType',
                                       'val.variable_type(var)')
        self.add_method('variable_type',
                        func_body=meth_variable_type,
                        arg_list=[UlongArg(name='var',
                                           cvarname='var')],
                        doc_str='return variable type')

        def meth_index_num(writer):
            writer.gen_return_py_ulong('val.index_num(var)')
        self.add_method('index_num',
                        func_body=meth_index_num,
                        arg_list=[UlongArg(name='var',
                                           cvarname='var')],
                        doc_str='return index size')

        def meth_index(writer):
            writer.gen_return_py_float('val.index(var, pos)')
        self.add_method('index',
                        func_body=meth_index,
                        arg_list=[UlongArg(name='var',
                                           cvarname='var'),
                                  UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return index value')

        def meth_grid_value(writer):
            writer.gen_vardecl(typename='std::vector<SizeType>',
                               varname='pos_array')
            with writer.gen_if_block('PyList<SizeType, PyUlong>::FromPyObject(tmp_obj, pos_array)'):
                writer.gen_return_py_float('val.grid_value(pos_array)')
            with writer.gen_else_block():
                writer.gen_type_error('"1st argument should be a list of integers"')
        self.add_method('grid_value',
                        func_body=meth_grid_value,
                        arg_list=[RawObjArg(name='pos_array',
                                            cvarname='tmp_obj')],
                        doc_str='return grid_value')

        def meth_value(writer):
            writer.gen_vardecl(typename='std::vector<double>',
                               varname='val_array')
            with writer.gen_if_block('PyList<double, PyFloat>::FromPyObject(tmp_obj, val_array)'):
                writer.gen_return_py_float('val.value(val_array)')
            with writer.gen_else_block():
                writer.gen_type_error('"1st argument should be a list of floats"')
        self.add_method('value',
                        func_body=meth_value,
                        arg_list=[RawObjArg(name='val_array',
                                            cvarname='tmp_obj')],
                        doc_str='return value')

        self.add_conv('default')
        self.add_deconv('default')
