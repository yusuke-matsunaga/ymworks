#! /usr/bin/env python3

""" PyAigMgr を生成するスクリプト

:file: aigmgr_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg
from logic import PrimTypeArg, ExprArg, ExprListArg
from logic import SopCoverArg, SopCubeArg
from bn import BnModelArg
from .aig_arg import AigLiteralArg, AigLiteralListArg

class AigMgrGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='AigMgr',
                         pyname='AigMgr',
                         namespace='YM',
                         header_include_files=['ym/AigMgr.h'],
                         source_include_files=['pym/PyAigMgr.h',
                                               'pym/PyAigLiteral.h',
                                               'pym/PyBnModel.h',
                                               'pym/PyPrimType.h',
                                               'pym/PyExpr.h',
                                               'pym/PySopCover.h',
                                               'pym/PySopCube.h',
                                               'pym/pyfstream.h',
                                               'pym/PyUlong.h',
                                               'pym/PyString.h',
                                               'pym/PyList.h'])

        self.add_dealloc('default')

        def meth_read_aag(writer):
            writer.gen_return_pyobject('PyAigMgr', 'AigMgr::read_aag(filename)')
        self.add_static_method('read_aag',
                               func_body=meth_read_aag,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               doc_str='read AAG file')

        def meth_read_aig(writer):
            writer.gen_return_pyobject('PyAigMgr', 'AigMgr::read_aig(filename)')
        self.add_static_method('read_aig',
                               func_body=meth_read_aig,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               doc_str='read AIG file')

        def meth_read_blif(writer):
            writer.gen_return_pyobject('PyAigMgr', 'AigMgr::read_blif(filename)')
        self.add_static_method('read_blif',
                               func_body=meth_read_blif,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               doc_str='read BLIF file')

        def meth_read_iscas89(writer):
            writer.gen_return_pyobject('PyAigMgr', 'AigMgr::read_iscas89(filename)')
        self.add_static_method('read_iscas89',
                               func_body=meth_read_iscas89,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               doc_str='read ISCAS89(.bench) file')

        def meth_read(writer):
            writer.gen_return_pyobject('PyAigMgr', 'AigMgr::read(filename, format)')
        self.add_static_method('read',
                               func_body=meth_read,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename'),
                                         StringArg(name='format',
                                                   cvarname='format')],
                               doc_str='read file (format should be aag, aig, blif or iscas89)')

        def meth_from_bn(writer):
            writer.gen_return_pyobject('PyAigMgr', 'AigMgr::from_bn(model)')
        self.add_static_method('from_bn',
                               func_body=meth_from_bn,
                               arg_list=[BnModelArg(name='model',
                                                    cvarname='model')],
                               doc_str='convert from BnModel')

        def meth_write_aag(writer):
            writer.gen_auto_assign('s', 'opyfstream(fout_obj)')
            writer.write_line('val.write_aag(s);')
            writer.gen_return_py_none()
        self.add_method('write_aag',
                        func_body=meth_write_aag,
                        arg_list=[RawObjArg(name='fout',
                                            cvarname='fout_obj')],
                        doc_str='write file in AAG format')

        def meth_write_aig(writer):
            writer.gen_auto_assign('s', 'opyfstream(fout_obj)')
            writer.write_line('val.write_aig(s);')
            writer.gen_return_py_none()
        self.add_method('write_aig',
                        func_body=meth_write_aig,
                        arg_list=[RawObjArg(name='fout',
                                            cvarname='fout_obj')],
                        doc_str='write file in AIG format')

        def meth_write(writer):
            writer.gen_auto_assign('s', 'opyfstream(fout_obj)')
            writer.write_line('val.write(s, format);')
            writer.gen_return_py_none()
        self.add_method('write',
                        func_body=meth_write,
                        arg_list=[RawObjArg(name='fout',
                                            cvarname='fout_obj'),
                                  StringArg(name='format',
                                            cvarname='format')],
                        doc_str='write file in specified format')

        def meth_copy(writer):
            writer.gen_return_pyobject('PyAigMgr', 'val.copy()')
        self.add_method('copy',
                        func_body=meth_copy,
                        doc_str='make a DEEP copy')

        def meth_input(writer):
            writer.gen_return_pyobject('PyAigLiteral', 'val.input(input_id)')
        self.add_method('input',
                        func_body=meth_input,
                        arg_list=[UlongArg(name='input_id',
                                           cvarname='input_id')],
                        doc_str='return Input Literal')

        def meth_input_name(writer):
            writer.gen_return_py_string('val.input_name(input_id)')
        self.add_method('input_name',
                        func_body=meth_input_name,
                        arg_list=[UlongArg(name='input_id',
                                           cvarname='input_id')],
                        doc_str='return Input Name')

        def meth_output(writer):
            writer.gen_return_pyobject('PyAigLiteral', 'val.output(output_id)')
        self.add_method('output',
                        func_body=meth_output,
                        arg_list=[UlongArg(name='output_id',
                                           cvarname='output_id')],
                        doc_str='return Output Literal')

        def meth_output_name(writer):
            writer.gen_return_py_string('val.output_name(output_id)')
        self.add_method('output_name',
                        func_body=meth_output_name,
                        arg_list=[UlongArg(name='output_id',
                                           cvarname='output_id')],
                        doc_str='return Output Name')

        def meth_dff_output(writer):
            writer.gen_return_pyobject('PyAigLiteral', 'val.dff_output(dff_id)')
        self.add_method('dff_output',
                        func_body=meth_dff_output,
                        arg_list=[UlongArg(name='dff_id',
                                           cvarname='dff_id')],
                        doc_str='return DFF Output Literal')

        def meth_dff_input(writer):
            writer.gen_return_pyobject('PyAigLiteral', 'val.dff_input(dff_id)')
        self.add_method('dff_input',
                        func_body=meth_dff_input,
                        arg_list=[UlongArg(name='dff_id',
                                           cvarname='dff_id')],
                        doc_str='return DFF Input Literal')

        def meth_dff_name(writer):
            writer.gen_return_py_string('val.dff_name(dff_id)')
        self.add_method('dff_name',
                        func_body=meth_dff_name,
                        arg_list=[UlongArg(name='dff_id',
                                           cvarname='dff_id')],
                        doc_str='return DFF Name')

        def meth_comment_list(writer):
            writer.gen_return_pyobject('PyList<std::string, PyString>', 'val.comment_list()')
        self.add_method('comment_list',
                        func_body=meth_comment_list,
                        doc_str='return list of comments')

        def meth_is_input(writer):
            writer.gen_return_py_bool('val.is_input(node_id)')
        self.add_method('is_input',
                        func_body=meth_is_input,
                        arg_list=[UlongArg(name='node_id',
                                           cvarname='node_id')],
                        doc_str='True if lit is INPUT')

        def meth_is_primary_input(writer):
            writer.gen_return_py_bool('val.is_primary_input(node_id)')
        self.add_method('is_primary_input',
                        func_body=meth_is_primary_input,
                        arg_list=[UlongArg(name='node_id',
                                           cvarname='node_id')],
                        doc_str='True if lit is PRIMARY INPUT')

        def meth_input_id(writer):
            writer.gen_return_py_ulong('val.input_id(node_id)')
        self.add_method('input_id',
                        func_body=meth_input_id,
                        arg_list=[UlongArg(name='node_id',
                                           cvarname='node_id')],
                        doc_str='return INPUT ID of lit')

        def meth_is_dff_output(writer):
            writer.gen_return_py_bool('val.is_dff_output(node_id)')
        self.add_method('is_dff_output',
                        func_body=meth_is_dff_output,
                        arg_list=[UlongArg(name='node_id',
                                           cvarname='node_id')],
                        doc_str='True if lit is DFF OUTPUT')

        def meth_dff_id(writer):
            writer.gen_return_py_ulong('val.dff_id(node_id)')
        self.add_method('dff_id',
                        func_body=meth_dff_id,
                        arg_list=[UlongArg(name='node_id',
                                           cvarname='node_id')],
                        doc_str='return DFF ID of lit')

        def meth_is_and(writer):
            writer.gen_return_py_bool('val.is_and(node_id)')
        self.add_method('is_and',
                        func_body=meth_is_and,
                        arg_list=[UlongArg(name='node_id',
                                           cvarname='node_id')],
                        doc_str='True if lit is AND')

        def meth_fanin(writer):
            writer.gen_return_pyobject('PyAigLiteral',
                                       'val.fanin(node_id, pos)')
        self.add_method('fanin',
                        func_body=meth_fanin,
                        arg_list=[UlongArg(name='node_id',
                                           cvarname='node_id'),
                                  UintArg(name='pos',
                                          cvarname='pos')],
                        doc_str='return fanin literal')

        def meth_fanin0(writer):
            writer.gen_return_pyobject('PyAigLiteral',
                                       'val.fanin0(node_id)')
        self.add_method('fanin0',
                        func_body=meth_fanin0,
                        arg_list=[UlongArg(name='node_id',
                                           cvarname='node_id')],
                        doc_str='return fanin0 literal')

        def meth_fanin1(writer):
            writer.gen_return_pyobject('PyAigLiteral',
                                       'val.fanin1(node_id)')
        self.add_method('fanin1',
                        func_body=meth_fanin1,
                        arg_list=[UlongArg(name='node_id',
                                           cvarname='node_id')],
                        doc_str='return fanin1 literal')

        def meth_ex_fanin_list(writer):
            writer.gen_return_pyobject('PyList<AigLiteral, PyAigLiteral>',
                                       'val.ex_fanin_list(node_id)')
        self.add_method('ex_fanin_list',
                        func_body=meth_ex_fanin_list,
                        arg_list=[UlongArg(name='node_id',
                                           cvarname='node_id')],
                        doc_str='return list of expanded fanin literals')

        def meth_new_input(writer):
            writer.gen_return_pyobject('PyAigLiteral',
                                       'val.new_input(name)')
        self.add_method('new_input',
                        func_body=meth_new_input,
                        arg_list=[OptArg(),
                                  KwdArg(),
                                  StringArg(name='name',
                                            cvarname='name')],
                        doc_str='make new Input')

        def meth_new_output(writer):
            writer.gen_return_py_ulong('val.new_output(lit, name)')
        self.add_method('new_output',
                        func_body=meth_new_output,
                        arg_list=[AigLiteralArg(name='lit',
                                                cvarname='lit'),
                                  OptArg(),
                                  KwdArg(),
                                  StringArg(name='name',
                                            cvarname='name')],
                        doc_str='make new Output')

        def meth_new_dff(writer):
            writer.gen_return_py_ulong('val.new_dff(name)')
        self.add_method('new_dff',
                        func_body=meth_new_dff,
                        arg_list=[OptArg(),
                                  KwdArg(),
                                  StringArg(name='name',
                                            cvarname='name')],
                        doc_str='make new DFF')

        def meth_set_dff_input(writer):
            writer.write_line('val.set_dff_input(dff_id, lit);')
            writer.gen_return_py_none()
        self.add_method('set_dff_input',
                        func_body=meth_set_dff_input,
                        arg_list=[UlongArg(name='dff_id',
                                           cvarname='dff_id'),
                                  AigLiteralArg(name='lit',
                                                cvarname='lit')],
                        doc_str='set DFF input')

        def meth_new_and(writer):
            writer.gen_return_pyobject('PyAigLiteral', 'val.new_and(fanin_list)')
        self.add_method('new_and',
                        func_body=meth_new_and,
                        arg_list=[AigLiteralListArg(name='fanin_list',
                                                    cvarname='fanin_list')],
                        doc_str='make new AND')

        def meth_new_or(writer):
            writer.gen_return_pyobject('PyAigLiteral', 'val.new_or(fanin_list)')
        self.add_method('new_or',
                        func_body=meth_new_or,
                        arg_list=[AigLiteralListArg(name='fanin_list',
                                                    cvarname='fanin_list')],
                        doc_str='make new OR')

        def meth_new_xor(writer):
            writer.gen_return_pyobject('PyAigLiteral', 'val.new_xor(fanin_list)')
        self.add_method('new_xor',
                        func_body=meth_new_xor,
                        arg_list=[AigLiteralListArg(name='fanin_list',
                                                    cvarname='fanin_list')],
                        doc_str='make new XOR')

        def meth_new_primitive(writer):
            writer.gen_return_pyobject('PyAigLiteral', 'val.new_primitive(prim_type, fanin_list)')
        self.add_method('new_primitive',
                        func_body=meth_new_primitive,
                        arg_list=[PrimTypeArg(name='prim_type',
                                              cvarname='prim_type'),
                                  AigLiteralListArg(name='fanin_list',
                                                    cvarname='fanin_list')],
                        doc_str='make new PRIMITIVE')

        def meth_from_expr(writer):
            with writer.gen_if_block('input_list.empty()'):
                writer.gen_return_pyobject('PyAigLiteral', 'val.from_expr(expr)')
            with writer.gen_else_block():
                writer.gen_return_pyobject('PyAigLiteral', 'val.from_expr(expr, input_list)')
        self.add_method('from_expr',
                        func_body=meth_from_expr,
                        arg_list=[ExprArg(name='expr',
                                          cvarname='expr'),
                                  OptArg(),
                                  KwdArg(),
                                  AigLiteralListArg(name='input_list',
                                                    cvarname='input_list')],
                        doc_str='make AIG from EXPR')

        def meth_from_expr_list(writer):
            with writer.gen_if_block('input_list.empty()'):
                writer.gen_return_pyobject('PyList<AigLiteral, PyAigLiteral>', 'val.from_expr_list(expr_list)')
            with writer.gen_else_block():
                writer.gen_return_pyobject('PyList<AigLiteral, PyAigLiteral>', 'val.from_expr_list(expr_list, input_list)')
        self.add_method('from_expr_list',
                        func_body=meth_from_expr_list,
                        arg_list=[ExprListArg(name='expr_list',
                                              cvarname='expr_list'),
                                  OptArg(),
                                  KwdArg(),
                                  AigLiteralListArg(name='input_list',
                                                    cvarname='input_list')],
                        doc_str='make AIG from EXPR list')

        def meth_from_cover(writer):
            with writer.gen_if_block('input_list.empty()'):
                writer.gen_return_pyobject('PyAigLiteral', 'val.from_cover(cover)')
            with writer.gen_else_block():
                writer.gen_return_pyobject('PyAigLiteral', 'val.from_cover(cover, input_list)')
        self.add_method('from_cover',
                        func_body=meth_from_cover,
                        arg_list=[SopCoverArg(name='cover',
                                              cvarname='cover'),
                                  OptArg(),
                                  KwdArg(),
                                  AigLiteralListArg(name='input_list',
                                                    cvarname='input_list')],
                        doc_str='make AIG from COVER')

        def meth_from_cube(writer):
            with writer.gen_if_block('input_list.empty()'):
                writer.gen_return_pyobject('PyAigLiteral', 'val.from_cube(cube)')
            with writer.gen_else_block():
                writer.gen_return_pyobject('PyAigLiteral', 'val.from_cube(cube, input_list)')
        self.add_method('from_cube',
                        func_body=meth_from_cube,
                        arg_list=[SopCubeArg(name='cube',
                                             cvarname='cube'),
                                  OptArg(),
                                  KwdArg(),
                                  AigLiteralListArg(name='input_list',
                                                    cvarname='input_list')],
                        doc_str='make AIG from CUBE')

        def meth_rewrite(writer):
            writer.write_line('val.rewrite();')
            writer.gen_return_py_none()
        self.add_method('rewrite',
                        func_body=meth_rewrite,
                        doc_str='do LOCAL REWRITING')

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

        def get_and_num(writer):
            writer.gen_return_py_ulong('val.and_num()')
        self.add_getter('get_and_num',
                        func_body=get_and_num)
        self.add_attr('and_num',
                      getter_name='get_and_num',
                      doc_str='number of AND nodes')

        def get_dff_num(writer):
            writer.gen_return_py_ulong('val.dff_num()')
        self.add_getter('get_dff_num',
                        func_body=get_dff_num)
        self.add_attr('dff_num',
                      getter_name='get_dff_num',
                      doc_str='number of DFFs')

        self.add_conv('default')
        self.add_deconv('default')
