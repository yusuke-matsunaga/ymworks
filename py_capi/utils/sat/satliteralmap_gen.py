#! /usr/bin/env python3

""" PySatLiteralMap を生成するスクリプト

:file: satliteralmap_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg
from .sat_arg import SatLiteralArg


class SatLiteralMapGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='SatLiteralMap',
                         pyclassname='PySatLiteralMap',
                         pyname='SatLiteralMap',
                         namespace='YM',
                         header_include_files=['ym/SatLiteral.h'],
                         source_include_files=['pym/PySatLiteralMap.h',
                                               'pym/PySatLiteral.h',
                                               'pym/PyUlong.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) SatLiteralMap();')
            with writer.gen_if_block('obj != nullptr'):
                writer.gen_auto_assign('emsg',
                                       '"1st argument should be a sequence of (integer, SatLiteral)"')
                with writer.gen_if_block('!PySequence_Check(obj)'):
                    writer.gen_type_error('emsg')
                with writer.gen_foreach_sequence_block('obj1', 'obj'):
                    writer.gen_vardecl(typename='PyObject*', varname='var_obj')
                    writer.gen_vardecl(typename='PyObject*', varname='lit_obj')
                    with writer.gen_if_block('!PyArg_ParseTuple(obj1, "(OO)", &var_obj, &lit_obj)' ):
                        writer.write_line('Py_DecRef(obj1);')
                        writer.gen_type_error('emsg')
                    with writer.gen_if_block('!PyUlong::Check(var_obj)'):
                        writer.write_line('Py_DecRef(obj1);')
                        writer.gen_type_error('emsg')
                    with writer.gen_if_block('!PySatLiteral::Check(lit_obj)'):
                        writer.write_line('Py_DecRef(obj1);')
                        writer.gen_type_error('emsg')
                    writer.gen_auto_assign('var', 'PyUlong::Get(var_obj)')
                    writer.gen_autoref_assign('lit', 'PySatLiteral::_get_ref(lit_obj)')
                    writer.write_line('my_obj->mVal.emplace(var, lit);')
                    writer.write_line('Py_DecRef(obj1);')
            writer.gen_return_self()
        self.add_new(new_body,
                     arg_list=[OptArg(),
                               RawObjArg(cvarname='obj',
                                         cvardefault='nullptr')])

        self.add_dealloc('default')

        def mp_length(writer):
            writer.gen_auto_assign('len_val', 'val.size()')
        def mp_subscript(writer):
            with writer.gen_if_block('!PyUlong::Check(key)'):
                writer.gen_type_error('"1st argument should be integer"')
            writer.gen_auto_assign('var', 'PyUlong::Get(key)')
            with writer.gen_if_block('val.count(var) == 0'):
                writer.gen_value_error('"key not found"')
            writer.gen_return_pyobject('PySatLiteral', 'val.at(var)')
        def mp_ass_subscript(writer):
            with writer.gen_if_block('!PyUlong::Check(key)'):
                writer.gen_type_error('"1st argument should be integer type"',
                                      error_val=-1)
            with writer.gen_if_block('!PySatLiteral::Check(obj)'):
                writer.gen_type_error('"2nd argument should be SatLiteral type"',
                                      error_val=-1)
            writer.gen_auto_assign('var', 'PyUlong::Get(key)')
            writer.gen_autoref_assign('lit', 'PySatLiteral::_get_ref(obj)')
            writer.write_line('val.emplace(var, lit);')
            writer.gen_return(0)
        self.add_mapping(mp_length=mp_length,
                         mp_subscript=mp_subscript,
                         mp_ass_subscript=mp_ass_subscript)

        self.add_deconv('default')
