#! /usr/bin/env python3

""" PyBnModel を生成するスクリプト

:file: bnmodel_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import CharArg, IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg
from misc import JsonValueArg
from logic import PrimTypeArg, SopCoverArg, ExprArg, TvFuncArg, BddArg
from .bn_arg import BnNodeArg, BnNodeListArg, BnDffArg


class BnModelGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BnModel',
                         pyname='BnModel',
                         namespace='YM',
                         header_include_files=['ym/BnModel.h'],
                         source_include_files=['pym/PyBnModel.h',
                                               'pym/PyBnDff.h',
                                               'pym/PyBnDffList.h',
                                               'pym/PyBnNode.h',
                                               'pym/PyBnNodeList.h',
                                               'pym/PyBnFunc.h',
                                               'pym/PyPrimType.h',
                                               'pym/PySopCover.h',
                                               'pym/PyExpr.h',
                                               'pym/PyTvFunc.h',
                                               'pym/PyBdd.h',
                                               'pym/PyJsonValue.h',
                                               'pym/pyfstream.h',
                                               'pym/PyUlong.h',
                                               'pym/PyString.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) BnModel();')
            writer.gen_return_self()
        self.add_new(new_body)

        self.add_dealloc('default')

        def meth_read_blif(writer):
            writer.gen_return_pyobject('PyBnModel', 'BnModel::read_blif(filename)')
        self.add_static_method('read_blif',
                               func_body=meth_read_blif,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               doc_str='read BLIF file')

        def meth_read_iscas89(writer):
            writer.gen_return_pyobject('PyBnModel', 'BnModel::read_iscas89(filename)')
        self.add_static_method('read_iscas89',
                               func_body=meth_read_iscas89,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               doc_str='read ISCAS89(.bench) file')

        def meth_read_truth(writer):
            writer.gen_return_pyobject('PyBnModel', 'BnModel::read_truth(filename)')
        self.add_static_method('read_truth',
                               func_body=meth_read_truth,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               doc_str='read TRUTH(.truth) file')

        def meth_read(writer):
            writer.gen_return_pyobject('PyBnModel', 'BnModel::read(filename, format)')
        self.add_static_method('read',
                               func_body=meth_read,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename'),
                                         OptArg(),
                                         KwdArg(),
                                         StringArg(name='format',
                                                   cvarname='format')],
                               doc_str='read file (format: [blif, iscas89, truth])')

        def meth_write(writer):
            writer.gen_auto_assign('s', 'opyfstream(fout_obj)')
            writer.gen_stmt('val.write(s)')
            writer.gen_return_py_none()
        self.add_method('write',
                        func_body=meth_write,
                        arg_list=[RawObjArg(name='fout',
                                            cvarname='fout_obj')],
                        doc_str='write')

        def meth_copy(writer):
            writer.gen_return_pyobject('PyBnModel', 'val.copy()')
        self.add_method('copy',
                        func_body=meth_copy,
                        doc_str='make a DEEP copy')

        def meth_dff(writer):
            writer.gen_return_pyobject('PyBnDff', 'val.dff(dff_id)')
        self.add_method('dff',
                        func_body=meth_dff,
                        arg_list=[IntArg(name='dff_id',
                                         cvarname='dff_id')],
                        doc_str='return DFF')

        def meth_dff_name(writer):
            writer.gen_return_py_string('val.dff_name(dff_id)')
        self.add_method('dff_name',
                        func_body=meth_dff_name,
                        arg_list=[IntArg(name='dff_id',
                                         cvarname='dff_id')],
                        doc_str='return DFF name')

        def meth_dff_list(writer):
            writer.gen_return_pyobject('PyBnDffList',
                                       'val.dff_list()')
        self.add_method('dff_list',
                        func_body=meth_dff_list,
                        doc_str='return list of DFFs')

        def meth_node(writer):
            writer.gen_return_pyobject('PyBnNode', 'val.node(node_id)')
        self.add_method('node',
                        func_body=meth_node,
                        arg_list=[IntArg(name='node_id',
                                         cvarname='node_id')],
                        doc_str='return BnNode')

        def meth_input(writer):
            writer.gen_return_pyobject('PyBnNode', 'val.input(input_id)')
        self.add_method('input',
                        func_body=meth_input,
                        arg_list=[IntArg(name='input_id',
                                         cvarname='input_id')],
                        doc_str='return Input Node')

        def meth_input_name(writer):
            writer.gen_return_py_string('val.input_name(input_id)')
        self.add_method('input_name',
                        func_body=meth_input_name,
                        arg_list=[IntArg(name='input_id',
                                         cvarname='input_id')],
                        doc_str='return Input Name')

        def meth_input_list(writer):
            writer.gen_return_pyobject('PyBnNodeList', 'val.input_list()')
        self.add_method('input_list',
                        func_body=meth_input_list,
                        doc_str='return list of inputs')

        def meth_output(writer):
            writer.gen_return_pyobject('PyBnNode', 'val.output(output_id)')
        self.add_method('output',
                        func_body=meth_output,
                        arg_list=[IntArg(name='output_id',
                                         cvarname='output_id')],
                        doc_str='return Output Node')

        def meth_output_name(writer):
            writer.gen_return_py_string('val.output_name(output_id)')
        self.add_method('output_name',
                        func_body=meth_output_name,
                        arg_list=[IntArg(name='output_id',
                                         cvarname='output_id')],
                        doc_str='return Output Name')

        def meth_output_list(writer):
            writer.gen_return_pyobject('PyBnNodeList', 'val.output_list()')
        self.add_method('output_list',
                        func_body=meth_output_list,
                        doc_str='return list of outputs')

        def meth_logic(writer):
            writer.gen_return_pyobject('PyBnNode', 'val.logic(id)')
        self.add_method('logic',
                        func_body=meth_logic,
                        arg_list=[IntArg(name='id',
                                         cvarname='id')],
                        doc_str='return Logic Node')

        def meth_logic_list(writer):
            writer.gen_return_pyobject('PyBnNodeList', 'val.logic_list()')
        self.add_method('logic_list',
                        func_body=meth_logic_list,
                        doc_str='return list of logic nodes')

        def meth_func(writer):
            writer.gen_return_pyobject('PyBnFunc', 'val.func(func_id)')
        self.add_method('func',
                        func_body=meth_func,
                        arg_list=[IntArg(name='func_id',
                                         cvarname='func_id')],
                        doc_str='return Function')

        def meth_comment_list(writer):
            writer.gen_return_pyobject('PyList<std::string, PyString>',
                                       'val.comment_list()')
        self.add_method('comment_list',
                        func_body=meth_comment_list,
                        doc_str='return list of comment lines')

        def get_name(writer):
            writer.gen_return_py_string('val.name()')
        self.add_getter('get_name',
                        func_body=get_name)
        self.add_attr('name',
                      getter_name='get_name',
                      doc_str='name')

        def get_dff_num(writer):
            writer.gen_return_py_ulong('val.dff_num()')
        self.add_getter('get_dff_num',
                        func_body=get_dff_num)
        self.add_attr('dff_num',
                      getter_name='get_dff_num',
                      doc_str='number of DFFs')

        def get_node_num(writer):
            writer.gen_return_py_ulong('val.node_num()')
        self.add_getter('get_node_num',
                        func_body=get_node_num)
        self.add_attr('node_num',
                      getter_name='get_node_num',
                      doc_str='number of all nodes')

        def get_input_num(writer):
            writer.gen_return_py_ulong('val.input_num()')
        self.add_getter('get_input_num',
                        func_body=get_input_num)
        self.add_attr('input_num',
                      getter_name='get_input_num',
                      doc_str='number of inputs')

        def get_output_num(writer):
            writer.gen_return_py_ulong('val.output_num()')
        self.add_getter('get_output_num',
                        func_body=get_output_num)
        self.add_attr('output_num',
                      getter_name='get_output_num',
                      doc_str='number of outputs')

        def get_logic_num(writer):
            writer.gen_return_py_ulong('val.logic_num()')
        self.add_getter('get_logic_num',
                        func_body=get_logic_num)
        self.add_attr('logic_num',
                      getter_name='get_logic_num',
                      doc_str='number of logic nodes')

        def get_func_num(writer):
            writer.gen_return_py_ulong('val.func_num()')
        self.add_getter('get_func_num',
                        func_body=get_func_num)
        self.add_attr('func_num',
                      getter_name='get_func_num',
                      doc_str='number of functions')

        def meth_set_option(writer):
            writer.gen_stmt('val.set_option(option)')
            writer.gen_return_py_none()
        self.add_method('set_option',
                        func_body=meth_set_option,
                        arg_list=[JsonValueArg(name='option',
                                               cvarname='option')],
                        doc_str='set option')

        def meth_set_name(writer):
            writer.gen_stmt('val.set_name(name)')
            writer.gen_return_py_none()
        self.add_method('set_name',
                        func_body=meth_set_name,
                        arg_list=[StringArg(name='name',
                                            cvarname='name')],
                        doc_str='set name')

        def meth_add_comment(writer):
            writer.gen_stmt('val.add_comment(comment)')
            writer.gen_return_py_none()
        self.add_method('add_comment',
                        func_body=meth_add_comment,
                        arg_list=[StringArg(name='comment',
                                            cvarname='comment')],
                        doc_str='add comment')

        def meth_new_dff(writer):
            writer.gen_return_pyobject('PyBnDff',
                                       'val.new_dff(name, reset_val)')
        self.add_method('new_dff',
                        func_body=meth_new_dff,
                        arg_list=[OptArg(),
                                  KwdArg(),
                                  StringArg(name='name',
                                            cvarname='name',
                                            cvardefault="{}"),
                                  CharArg(name='reset_val',
                                          cvarname='reset_val',
                                          cvardefault="'X'")],
                        doc_str='new DFF')

        def meth_set_dff_src(writer):
            writer.gen_stmt('val.set_dff_src(dff, src)')
            writer.gen_return_py_none()
        self.add_method('set_dff_src',
                        func_body=meth_set_dff_src,
                        arg_list=[BnDffArg(name='dff',
                                           cvarname='dff'),
                                  BnNodeArg(name='src',
                                            cvarname='src')],
                        doc_str='set DFF source input')

        def meth_new_input(writer):
            writer.gen_return_pyobject('PyBnNode',
                                       'val.new_input(name)')
        self.add_method('new_input',
                        func_body=meth_new_input,
                        arg_list=[OptArg(),
                                  KwdArg(),
                                  StringArg(name='name',
                                            cvarname='name',
                                            cvardefault='{}')],
                        doc_str='new input node')

        def meth_new_output(writer):
            writer.gen_return_py_ulong('val.new_output(src, name)')
        self.add_method('new_output',
                        func_body=meth_new_output,
                        arg_list=[BnNodeArg(name='src',
                                            cvarname='src'),
                                  OptArg(),
                                  KwdArg(),
                                  StringArg(name='name',
                                            cvarname='name',
                                            cvardefault='{}')],
                        doc_str='new output node')

        def meth_new_primitive(writer):
            writer.gen_return_pyobject('PyBnNode',
                                       'val.new_primitive(primitive_type, fanin_list)')
        self.add_method('new_primitive',
                        func_body=meth_new_primitive,
                        arg_list=[PrimTypeArg(name='primitive_type',
                                              cvarname='primitive_type'),
                                  BnNodeListArg(name='fanin_list',
                                                cvarname='fanin_list')],
                        doc_str='new primitive-type node')

        def meth_new_cover(writer):
            writer.gen_return_pyobject('PyBnNode',
                                       'val.new_cover(input_cover, output_inv, fanin_list)')
        self.add_method('new_cover',
                        func_body=meth_new_cover,
                        arg_list=[SopCoverArg(name='input_cover',
                                              cvarname='input_cover'),
                                  BoolArg(name='output_inv',
                                          cvarname='output_inv'),
                                  BnNodeListArg(name='fanin_list',
                                                cvarname='fanin_list')],
                        doc_str='new cover-type node')

        def meth_new_expr(writer):
            writer.gen_return_pyobject('PyBnNode',
                                       'val.new_expr(expr, fanin_list)')
        self.add_method('new_expr',
                        func_body=meth_new_expr,
                        arg_list=[ExprArg(name='expr',
                                          cvarname='expr'),
                                  BnNodeListArg(name='fanin_list',
                                                cvarname='fanin_list')],
                        doc_str='new expr-type node')

        def meth_new_tvfunc(writer):
            writer.gen_return_pyobject('PyBnNode',
                                       'val.new_tvfunc(func, fanin_list)')
        self.add_method('new_tvfunc',
                        func_body=meth_new_tvfunc,
                        arg_list=[TvFuncArg(name='func',
                                            cvarname='func'),
                                  BnNodeListArg(name='fanin_list',
                                                cvarname='fanin_list')],
                        doc_str='new tvfunc-type node')

        def meth_new_bdd(writer):
            writer.gen_return_pyobject('PyBnNode',
                                       'val.new_bdd(bdd, fanin_list)')
        self.add_method('new_bdd',
                        func_body=meth_new_bdd,
                        arg_list=[BddArg(name='bdd',
                                         cvarname='bdd'),
                                  BnNodeListArg(name='fanin_list',
                                                cvarname='fanin_list')],
                        doc_str='new BDD-type node')

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
