#! /usr/bin/env python3

""" PyTvFunc を生成するスクリプト

:file: tvfunc_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg
from .logic_arg import TvFuncArg, NpnMapArg


class TvFuncGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='TvFunc',
                         pyname='TvFunc',
                         namespace='YM',
                         header_include_files=['ym/TvFunc.h'],
                         source_include_files=['pym/PyTvFunc.h',
                                               'pym/PyLiteral.h',
                                               'pym/PyNpnMap.h',
                                               'pym/PyPrimType.h',
                                               'pym/PySopCover.h',
                                               'pym/PyInt.h',
                                               'pym/PyLong.h',
                                               'pym/PyUlong.h',
                                               'pym/PyString.h',
                                               'pym/PyModule.h',
                                               'ym/Tv2Sop.h'])

        def new_body(writer):
            writer.gen_vardecl(typename='TvFunc',
                               varname='func')
            writer.gen_vardecl(typename='std::vector<int>',
                               varname='vect')
            with writer.gen_if_block('ni == -1'):
                writer.gen_assign('func', 'TvFunc::invalid()')
            with writer.gen_elseif_block('vect_obj == nullptr'):
                writer.gen_assign('func', 'TvFunc::zero(ni)')
            with writer.gen_elseif_block('PyList<int, PyInt>::FromPyObject(vect_obj, vect)'):
                writer.gen_auto_assign('n', 'vect.size()')
                with writer.gen_if_block('n != (1 << ni)'):
                    writer.gen_value_error('"invalid vector size"')
                writer.gen_assign('func', 'TvFunc(ni, vect)')
            with writer.gen_else_block():
                writer.gen_value_error('"argument 2 must be a sequence of ints"')
            writer.gen_return_pyobject('PyTvFunc', 'std::move(func)')
        self.add_new(func_body=new_body,
                     arg_list=[OptArg(),
                               IntArg(name='input_num',
                                      cvarname='ni',
                                      cvardefault='-1'),
                               KwdArg(),
                               RawObjArg(name='values',
                                         cvarname='vect_obj')])

        self.add_dealloc()

        def repr_func(writer):
            writer.gen_return_py_string('val.str()')
        self.add_repr(func_body=repr_func)

        def meth_from_string(writer):
            writer.gen_return_pyobject('PyTvFunc', 'TvFunc(s, mode)')
        self.add_static_method('from_str',
                               func_body=meth_from_string,
                               arg_list=[StringArg(name='str',
                                                   cvarname='s'),
                                         OptArg(),
                                         KwdArg(),
                                         IntArg(name='mode',
                                                cvarname='mode',
                                                cvardefault=2)],
                               has_catch_block=True,
                               doc_str='make TvFunc object from string')

        def meth_from_binstring(writer):
            writer.gen_return_pyobject('PyTvFunc', 'TvFunc(s, 2)')
        self.add_static_method('from_bin_str',
                               func_body=meth_from_binstring,
                               arg_list=[StringArg(name='str',
                                                   cvarname='s'),],
                               has_catch_block=True,
                               doc_str='make TvFunc object from BIN string')

        def meth_from_hexstring(writer):
            writer.gen_return_pyobject('PyTvFunc', 'TvFunc(s, 16)')
        self.add_static_method('from_hex_str',
                               func_body=meth_from_hexstring,
                               arg_list=[StringArg(name='str',
                                                   cvarname='s'),],
                               has_catch_block=True,
                               doc_str='make TvFunc object from HEX string')

        def meth_invalid(writer):
            writer.gen_return_pyobject('PyTvFunc', 'TvFunc::invalid()')
        self.add_static_method('invalid',
                               func_body=meth_invalid,
                               doc_str='make invalid TvFunc object')

        def meth_zero(writer):
            writer.gen_return_pyobject('PyTvFunc', 'TvFunc::zero(ni)')
        self.add_static_method('zero',
                               func_body=meth_zero,
                               arg_list=[IntArg(name='input_num',
                                                cvarname='ni',
                                                cvardefault='0')],
                               doc_str='make ZERO TvFunc object')

        def meth_one(writer):
            writer.gen_return_pyobject('PyTvFunc', 'TvFunc::one(ni)')
        self.add_static_method('one',
                               func_body=meth_one,
                               arg_list=[IntArg(name='input_num',
                                                cvarname='ni',
                                                cvardefault='0')],
                               doc_str='make ONE TvFunc object')

        def meth_literal(writer):
            writer.gen_vardecl(typename='Literal',
                               varname='lit')
            with writer.gen_if_block('PyLiteral::FromPyObject(lit_obj, lit)'):
                with writer.gen_if_block('lit.varid() >= ni'):
                    writer.gen_value_error('"\'lit.varid()\' is out of range"')
                with writer.gen_if_block('inv'):
                    writer.gen_assign('lit', '~lit')
                writer.gen_return_pyobject('PyTvFunc', 'TvFunc::literal(ni, lit)')
            writer.gen_value_error('"argument 2 must be an integer or a Literal object"')
        self.add_static_method('literal',
                               func_body=meth_literal,
                               arg_list=[IntArg(name='input_num',
                                                cvarname='ni'),
                                         RawObjArg(name='lit',
                                                   cvarname='lit_obj'),
                                         OptArg(),
                                         BoolArg(name='inv',
                                                 cvarname='inv',
                                                 cvardefault='false')],
                               doc_str='make literal type TvFunc object')

        def meth_positive_literal(writer):
            with writer.gen_if_block('var >= ni'):
                writer.gen_value_error('"\'var\' is out of range"')
            writer.gen_return_pyobject('PyTvFunc', 'TvFunc::positive_literal(ni, var)')
        self.add_static_method('positive_literal',
                               func_body=meth_positive_literal,
                               arg_list=[IntArg(name='input_num',
                                                cvarname='ni'),
                                         IntArg(name='var',
                                                cvarname='var')],
                               doc_str='make positive literal type TvFunc object')

        def meth_negative_literal(writer):
            with writer.gen_if_block('var >= ni'):
                writer.gen_value_error('"\'var\' is out of range"')
            writer.gen_return_pyobject('PyTvFunc', 'TvFunc::negative_literal(ni, var)')
        self.add_static_method('negative_literal',
                               func_body=meth_negative_literal,
                               arg_list=[IntArg(name='input_num',
                                                cvarname='ni'),
                                         IntArg(name='var',
                                                cvarname='var')],
                               doc_str='make negative literal type TvFunc object')

        def meth_cofactor(writer):
            writer.gen_vardecl(typename='Literal',
                               varname='lit')
            with writer.gen_if_block('PyLiteral::FromPyObject(lit_obj, lit)'):
                with writer.gen_if_block('lit.varid() >= val.input_num()'):
                    writer.gen_value_error('"\'lit.varid()\' is out of range"')
                with writer.gen_if_block('inv'):
                    writer.gen_assign('lit', '~lit')
                writer.gen_return_pyobject('PyTvFunc', 'val.cofactor(lit)')
            writer.gen_value_error('"argument 1 must be an integer or a Literal object"')
        self.add_method('cofactor',
                        func_body=meth_cofactor,
                        arg_list=[RawObjArg(name='lit',
                                            cvarname='lit_obj'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv',
                                          cvardefault='false')],
                        doc_str='make a cofactor')

        def meth_cofactor_int(writer):
            writer.gen_vardecl(typename='Literal',
                               varname='lit')
            with writer.gen_if_block('PyLiteral::FromPyObject(lit_obj, lit)'):
                with writer.gen_if_block('lit.varid() >= val.input_num()'):
                    writer.gen_value_error('"\'lit.varid()\' is out of range"')
                with writer.gen_if_block('inv'):
                    writer.gen_assign('lit', '~lit')
                writer.write_line('val.cofactor_int(lit);')
                writer.gen_return_self(incref=True)
            writer.gen_value_error('"argument 1 must be an integer or a Literal object"')
        self.add_method('cofactor_int',
                        func_body=meth_cofactor_int,
                        arg_list=[RawObjArg(name='lit',
                                            cvarname='lit_obj'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv',
                                          cvardefault='false')],
                        doc_str='make a cofactor')

        def meth_xform(writer):
            writer.gen_return_pyobject('PyTvFunc', 'val.xform(npnmap)')
        self.add_method('xform',
                        func_body=meth_xform,
                        arg_list=[NpnMapArg(name='map',
                                            cvarname='npnmap')],
                        doc_str='transform')

        def meth_shrink_map(writer):
            writer.gen_return_pyobject('PyNpnMap', 'val.shrink_map()')
        self.add_method('shrink_map',
                        func_body=meth_shrink_map,
                        doc_str='shrink unused inputs')

        def meth_npn_canonical_map(writer):
            writer.gen_return_pyobject('PyNpnMap', 'val.npn_canonical_map()')
        self.add_method('npn_canonical_map',
                        func_body=meth_npn_canonical_map,
                        doc_str='get canonical map')

        def meth_npn_canonical_all_map(writer):
            writer.gen_return_pyobject('PyList<NpnMap, PyNpnMap>', 'val.npn_canonical_all_map()')
        self.add_method('npn_canonical_all_map',
                        func_body=meth_npn_canonical_all_map,
                        doc_str='get list of all canonical maps')

        def meth_is_valid(writer):
            writer.gen_return_py_bool('val.is_valid()')
        self.add_method('is_valid',
                        func_body=meth_is_valid,
                        doc_str='True if valid')

        def meth_is_invalid(writer):
            writer.gen_return_py_bool('val.is_invalid()')
        self.add_method('is_invalid',
                        func_body=meth_is_invalid,
                        doc_str='True if invalid')

        def meth_is_zero(writer):
            writer.gen_return_py_bool('val.is_zero()')
        self.add_method('is_zero',
                        func_body=meth_is_zero,
                        doc_str='True if ZERO')

        def meth_is_one(writer):
            writer.gen_return_py_bool('val.is_one()')
        self.add_method('is_one',
                        func_body=meth_is_one,
                        doc_str='True if ONE')

        def get_input_num(writer):
            writer.gen_return_py_ulong('val.input_num()')
        self.add_getter('get_input_num',
                        func_body=get_input_num)
        self.add_attr('input_num',
                      getter_name='get_input_num',
                      doc_str='number of inputs')

        def meth_value(writer):
            writer.gen_auto_assign('nexp', '1 << val.input_num()')
            with writer.gen_if_block('index >= nexp'):
                writer.gen_value_error('"\'index\' is out of range"')
            writer.gen_return_py_int('val.value(index)')
        self.add_method('value',
                        func_body=meth_value,
                        arg_list=[IntArg(name='index',
                                         cvarname='index')],
                        doc_str='return truth table value at the specified index')

        def meth_count_zero(writer):
            writer.gen_return_py_long('val.count_zero()')
        self.add_method('count_zero',
                        func_body=meth_count_zero,
                        doc_str='return the number of minterms of ZERO')

        def meth_count_one(writer):
            writer.gen_return_py_long('val.count_one()')
        self.add_method('count_one',
                        func_body=meth_count_one,
                        doc_str='return the number of minterms of ONE')

        def meth_walsh_0(writer):
            writer.gen_return_py_long('val.walsh_0()')
        self.add_method('walsh_0',
                        func_body=meth_walsh_0,
                        doc_str='return Walsh-0 coefficient')

        def meth_walsh_1(writer):
            with writer.gen_if_block('var >= val.input_num()'):
                writer.gen_value_error('"\'var\' is out of range"')
            writer.gen_return_py_long('val.walsh_1(var)')
        self.add_method('walsh_1',
                        func_body=meth_walsh_1,
                        arg_list=[IntArg(name='var',
                                         cvarname='var')],
                        doc_str='return Walsh-1 coefficient')

        def meth_walsh_2(writer):
            with writer.gen_if_block('var1 >= val.input_num()'):
                writer.gen_value_error('"\'var1\' is out of range"')
            with writer.gen_if_block('var2 >= val.input_num()'):
                writer.gen_value_error('"\'var2\' is out of range"')
            writer.gen_return_py_long('val.walsh_2(var1, var2)')
        self.add_method('walsh_2',
                        func_body=meth_walsh_2,
                        arg_list=[IntArg(name='var1',
                                         cvarname='var1'),
                                  IntArg(name='var2',
                                         cvarname='var2')],
                        doc_str='return Walsh-2 coefficient')

        def meth_check_sup(writer):
            with writer.gen_if_block('var >= val.input_num()'):
                writer.gen_value_error('"\'var\' is out of range"')
            writer.gen_return_py_bool('val.check_sup(var)')
        self.add_method('check_sup',
                        func_body=meth_check_sup,
                        arg_list=[IntArg(name='var',
                                         cvarname='var')],
                        doc_str='check if the specified variable is a support')

        def meth_check_unate(writer):
            with writer.gen_if_block('var >= val.input_num()'):
                writer.gen_value_error('"\'var\' is out of range"')
            writer.gen_return_py_bool('val.check_unate(var)')
        self.add_method('check_unate',
                        func_body=meth_check_unate,
                        arg_list=[IntArg(name='var',
                                         cvarname='var')],
                        doc_str='check unateness on the specified variable')

        def meth_check_sym(writer):
            with writer.gen_if_block('var1 >= val.input_num()'):
                writer.gen_value_error('"\'var1\' is out of range"')
            with writer.gen_if_block('var2 >= val.input_num()'):
                writer.gen_value_error('"\'var2\' is out of range"')
            writer.gen_return_py_bool('val.check_sym(var1, var2, inv)')
        self.add_method('check_sym',
                        func_body=meth_check_sym,
                        arg_list=[IntArg(name='var1',
                                         cvarname='var1'),
                                  IntArg(name='var2',
                                         cvarname='var2'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv',
                                          cvardefault='false')],
                        doc_str='check synmmetry')

        def meth_analyze(writer):
            writer.gen_return_pyobject('PyPrimType', 'val.analyze()')
        self.add_method('analyze',
                        func_body=meth_analyze,
                        doc_str='analyze the function type')

        def meth_all_primes(writer):
            with writer.gen_if_block('dc.is_invalid()'):
                writer.gen_auto_assign('cube_list', 'Tv2Sop::all_primes(f)')
                writer.gen_return_pyobject('PySopCover',
                                           'SopCover(f.input_num(), cube_list)')
            with writer.gen_else_block():
                writer.gen_auto_assign('cube_list', 'Tv2Sop::all_primes(f, dc)')
                writer.gen_return_pyobject('PySopCover',
                                           'SopCover(f.input_num(), cube_list)')
        self.add_static_method('all_primes',
                               func_body=meth_all_primes,
                               arg_list=[TvFuncArg(name='f',
                                                   cvarname='f'),
                                         OptArg(),
                                         TvFuncArg(name='dc',
                                                   cvarname='dc')],
                               doc_str='return all primes')

        def meth_isop(writer):
            with writer.gen_if_block('dc.is_invalid()'):
                writer.gen_auto_assign('cube_list', 'Tv2Sop::isop(f)')
                writer.gen_return_pyobject('PySopCover',
                                           'SopCover(f.input_num(), cube_list)')
            with writer.gen_else_block():
                writer.gen_auto_assign('cube_list', 'Tv2Sop::isop(f, dc)')
                writer.gen_return_pyobject('PySopCover',
                                           'SopCover(f.input_num(), cube_list)')
        self.add_static_method('isop',
                               func_body=meth_isop,
                               arg_list=[TvFuncArg(name='f',
                                                   cvarname='f'),
                                         OptArg(),
                                         TvFuncArg(name='dc',
                                                   cvarname='dc')],
                               doc_str='return irredundant SOP')

        def get_bin_str(writer):
            writer.gen_return_py_string('val.bin_str()')
        self.add_getter('get_bin_str',
                        func_body=get_bin_str)
        self.add_attr('bin_str',
                      getter_name='get_bin_str',
                      doc_str='BIN string')

        def get_hex_str(writer):
            writer.gen_return_py_string('val.hex_str()')
        self.add_getter('get_hex_str',
                        func_body=get_hex_str)
        self.add_attr('hex_str',
                      getter_name='get_hex_str',
                      doc_str='HEX string')

        self.add_richcompare('eq_default')

        self.add_nb_invert()
        self.add_nb_and()
        self.add_nb_or()
        self.add_nb_xor()
        self.add_nb_inplace_and()
        self.add_nb_inplace_or()
        self.add_nb_inplace_xor()

        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(hash_func)

        def conv_func(writer):
            self.gen_alloc_code(writer, varname='obj')
            self.gen_obj_conv(writer, objname='obj', varname='my_obj')
            writer.write_line(f'new (&my_obj->mVal) {self.classname}(std::move(val));')
            writer.gen_return('obj')
        self.add_conv(conv_func)

        self.add_deconv('default')


if __name__ == '__main__':

    gen = TvFuncGen()

    gen.make_header()

    gen.make_source()
