#! /usr/bin/env python3

""" PySatSolver を生成するスクリプト

:file: satsolver_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import IntArg, UlongArg, BoolArg
from mk_py_capi import RawObjArg, TypedRawObjArg, TypedObjConvArg
from mk_py_capi import OptArg, KwdArg
from .sat_arg import SatLiteralArg, SatLiteralListArg, SatLiteralMapArg
from logic import ExprArg
from aig import AigMgrArg


class LitsParser:
    """
    １つ以上の SatLiteral のシーケンスを読み込むパーサークラス
    特例として SatLiteral 単体のオブジェクトも受け付ける．
    """

    def __init__(self, *,
                 cvarname):
        self.__cvarname = cvarname

    def has_args(self):
        return True

    def has_keywords(self):
        return False

    def __call__(self, writer):
        writer.gen_vardecl(typename='std::vector<SatLiteral>',
                           varname=self.__cvarname)
        with writer.gen_if_block(f'!parse_lits(args, {self.__cvarname})'):
            writer.gen_return('nullptr')


class LitLitsParser:
    """
    (出力を表す)SatLiteral と 1つ以上の SatLiteral のシーケンスを
    読み込むパーサークラス
    """

    def __init__(self, *,
                 ovarname,
                 cvarname):
        self.__ovarname = ovarname
        self.__cvarname = cvarname

    def has_args(self):
        return True

    def has_keywords(self):
        return False

    def __call__(self, writer):
        writer.gen_vardecl(typename='SatLiteral',
                           varname=self.__ovarname)
        writer.gen_vardecl(typename='std::vector<SatLiteral>',
                           varname=self.__cvarname)
        with writer.gen_if_block(f'!parse_lit_lits(args, {self.__ovarname}, {self.__cvarname})'):
            writer.gen_return('nullptr')


class SatSolverGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='SatSolver',
                         pyname='SatSolver',
                         namespace='YM',
                         header_include_files=['ym/SatSolver.h'],
                         source_include_files=['pym/PySatSolver.h',
                                               'pym/PySatInitParam.h',
                                               'pym/PySatBool3.h',
                                               'pym/PySatLiteral.h',
                                               'pym/PySatLiteralList.h',
                                               'pym/PySatLiteralMap.h',
                                               'pym/PySatCondBlock.h',
                                               'pym/PySatModel.h',
                                               'pym/PyExpr.h',
                                               'pym/PyAigMgr.h',
                                               'pym/PyInt.h',
                                               'pym/PyUlong.h',
                                               ])

        def preamble_body(writer):
            writer.gen_include('parse_lits.cc')
        self.add_preamble(preamble_body)

        def new_body(writer):
            with writer.gen_if_block('obj == nullptr'):
                writer.gen_tp_alloc(objclass='SatSolver_Object')
                writer.gen_stmt('new (&my_obj->mVal) SatSolver()')
                writer.gen_return_self()
            with writer.gen_else_block():
                writer.gen_autoref_assign('init_param',
                                          'PySatInitParam::_get_ref(obj)')
                writer.gen_tp_alloc(objclass='SatSolver_Object')
                writer.gen_stmt('new (&my_obj->mVal) SatSolver(init_param)')
                writer.gen_return_self()
        self.add_new(func_body=new_body,
                     arg_list=[OptArg(),
                               TypedRawObjArg(name='init_param',
                                              pytypename='PySatInitParam::_typeobject()',
                                              cvarname='obj',
                                              cvardefault='nullptr')])

        self.add_dealloc('default')

        def meth_new_variable(writer):
            writer.gen_return_pyobject('PySatLiteral',
                                       'val.new_variable(decision)')
        self.add_method('new_variable',
                        func_body=meth_new_variable,
                        arg_list=[OptArg(),
                                  BoolArg(name='decision',
                                          cvarname='decision',
                                          cvardefault='true')],
                        doc_str='make new variable')

        def meth_add_clause(writer):
            writer.gen_stmt('val.add_clause(lits_list)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_clause',
                                    func_body=meth_add_clause,
                                    arg_parser=LitsParser(cvarname='lits_list'),
                                    doc_str='add clause')

        def meth_add_buffgate(writer):
            writer.gen_stmt('val.add_buffgate(lit1, lit2)')
            writer.gen_return_py_none()
        self.add_method('add_buffgate',
                        func_body=meth_add_buffgate,
                        arg_list=[SatLiteralArg(name='lit1',
                                                cvarname='lit1'),
                                  SatLiteralArg(name='lit2',
                                                cvarname='lit2')],
                        doc_str="add clauses representing 'BUF' gate")

        def meth_add_notgate(writer):
            writer.gen_stmt('val.add_notgate(lit1, lit2)')
            writer.gen_return_py_none()
        self.add_method('add_notgate',
                        func_body=meth_add_notgate,
                        arg_list=[SatLiteralArg(name='lit1',
                                                cvarname='lit1'),
                                  SatLiteralArg(name='lit2',
                                                cvarname='lit2')],
                        doc_str="add clauses representing 'NOT' gate")

        def meth_add_andgate(writer):
            writer.gen_stmt('val.add_andgate(olit, lits_list)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_andgate',
                                    func_body=meth_add_andgate,
                                    arg_parser=LitLitsParser(ovarname='olit',
                                                             cvarname='lits_list'),
                                    doc_str="add clauses representing 'AND' gate")

        def meth_add_nandgate(writer):
            writer.gen_stmt('val.add_nandgate(olit, lits_list)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_nandgate',
                                    func_body=meth_add_nandgate,
                                    arg_parser=LitLitsParser(ovarname='olit',
                                                             cvarname='lits_list'),
                                    doc_str="add clauses representing 'NAND' gate")

        def meth_add_orgate(writer):
            writer.gen_stmt('val.add_orgate(olit, lits_list)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_orgate',
                                    func_body=meth_add_orgate,
                                    arg_parser=LitLitsParser(ovarname='olit',
                                                             cvarname='lits_list'),
                                    doc_str="add clauses representing 'OR' gate")

        def meth_add_norgate(writer):
            writer.gen_stmt('val.add_norgate(olit, lits_list)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_norgate',
                                    func_body=meth_add_norgate,
                                    arg_parser=LitLitsParser(ovarname='olit',
                                                             cvarname='lits_list'),
                                    doc_str="add clauses representing 'NOR' gate")

        def meth_add_xorgate(writer):
            writer.gen_stmt('val.add_xorgate(olit, lits_list)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_xorgate',
                                    func_body=meth_add_xorgate,
                                    arg_parser=LitLitsParser(ovarname='olit',
                                                             cvarname='lits_list'),
                                    doc_str="add clauses representing 'XOR' gate")

        def meth_add_xnorgate(writer):
            writer.gen_stmt('val.add_xnorgate(olit, lits_list)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_xnorgate',
                                    func_body=meth_add_xnorgate,
                                    arg_parser=LitLitsParser(ovarname='olit',
                                                             cvarname='lits_list'),
                                    doc_str="add clauses representing 'XNOR' gate")

        def meth_add_expr(writer):
            writer.gen_return_pyobject('PySatLiteralList',
                                       'val.add_expr(expr, lit_map)')
        self.add_method('add_expr',
                        func_body=meth_add_expr,
                        arg_list=[ExprArg(name='expr',
                                          cvarname='expr'),
                                  SatLiteralMapArg(name='lit_map',
                                                   cvarname='lit_map')],
                        doc_str='add clauses representing given Expr')

        def meth_add_aig(writer):
            writer.gen_return_pyobject('PyList<std::vector<SatLiteral>, PySatLiteralList>',
                                       'val.add_aig(aig_mgr, lit_map)')
        self.add_method('add_aig',
                        func_body=meth_add_aig,
                        arg_list=[AigMgrArg(name='aig_mgr',
                                            cvarname='aig_mgr'),
                                  SatLiteralMapArg(name='lit_map',
                                                   cvarname='lit_map')],
                        doc_str='add clauses representing given AIG')

        def meth_add_half_adder(writer):
            writer.gen_stmt('val.add_half_adder(alit, blit, slit, clit)')
            writer.gen_return_py_none()
        self.add_method('add_half_adder',
                        func_body=meth_add_half_adder,
                        arg_list=[SatLiteralArg(name='a',
                                                cvarname='alit'),
                                  SatLiteralArg(name='b',
                                                cvarname='blit'),
                                  SatLiteralArg(name='s',
                                                cvarname='slit'),
                                  SatLiteralArg(name='c',
                                                cvarname='clit')],
                        doc_str="add clauses representing 'HALF ADDER'")

        def meth_add_full_adder(writer):
            writer.gen_stmt('val.add_full_adder(alit, blit, ilit, slit, clit)')
            writer.gen_return_py_none()
        self.add_method('add_full_adder',
                        func_body=meth_add_full_adder,
                        arg_list=[SatLiteralArg(name='a',
                                                cvarname='alit'),
                                  SatLiteralArg(name='b',
                                                cvarname='blit'),
                                  SatLiteralArg(name='i',
                                                cvarname='ilit'),
                                  SatLiteralArg(name='s',
                                                cvarname='slit'),
                                  SatLiteralArg(name='c',
                                                cvarname='clit')],
                        doc_str="add clauses representing 'FULL ADDER'")

        def meth_add_adder(writer):
            writer.gen_stmt('val.add_adder(alits, blits, ilit, slits, clit)')
            writer.gen_return_py_none()
        self.add_method('add_adder',
                        func_body=meth_add_adder,
                        arg_list=[SatLiteralListArg(name='a',
                                                    cvarname='alits'),
                                  SatLiteralListArg(name='b',
                                                    cvarname='blits'),
                                  SatLiteralArg(name='i',
                                                cvarname='ilit'),
                                  SatLiteralListArg(name='s',
                                                    cvarname='slits'),
                                  SatLiteralArg(name='c',
                                                cvarname='clit')],
                        doc_str="add clauses representing 'ADDER'")

        def meth_add_counter(writer):
            writer.gen_return_pyobject('PySatLiteralList',
                                       'val.add_counter(ilits, decision)')
        self.add_method('add_counter',
                        func_body=meth_add_counter,
                        arg_list=[SatLiteralListArg(name='ilits',
                                                    cvarname='ilits'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='decision',
                                          cvarname='decision',
                                          cvardefault='false')],
                        doc_str="add clauses representing 'COUNTER'")

        def meth_add_at_most_one(writer):
            writer.gen_stmt('val.add_at_most_one(lits)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_at_most_one',
                                    func_body=meth_add_at_most_one,
                                    arg_parser=LitsParser(cvarname='lits'),
                                    doc_str="add clauses representing 'AT MOST ONE'")

        def meth_add_at_most_two(writer):
            writer.gen_stmt('val.add_at_most_two(lits)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_at_most_two',
                                    func_body=meth_add_at_most_two,
                                    arg_parser=LitsParser(cvarname='lits'),
                                    doc_str="add clauses representing 'AT MOST TWO'")

        def meth_add_at_most_k(writer):
            writer.gen_stmt('val.add_at_most_k(lits, k)')
            writer.gen_return_py_none()
        self.add_method('add_at_most_k',
                        func_body=meth_add_at_most_k,
                        arg_list=[SatLiteralListArg(name='lits',
                                                    cvarname='lits'),
                                  UlongArg(name='k',
                                           cvarname='k')],
                        doc_str="add clauses representing 'AT MOST K'")

        def meth_add_at_least_one(writer):
            writer.gen_stmt('val.add_at_least_one(lits)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_at_least_one',
                                    func_body=meth_add_at_least_one,
                                    arg_parser=LitsParser(cvarname='lits'),
                                    doc_str="add clauses representing 'AT LEAST ONE'")

        def meth_add_at_least_two(writer):
            writer.gen_stmt('val.add_at_least_two(lits)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_at_least_two',
                                    func_body=meth_add_at_least_two,
                                    arg_parser=LitsParser(cvarname='lits'),
                                    doc_str="add clauses representing 'AT LEAST TWO'")

        def meth_add_at_least_k(writer):
            writer.gen_stmt('val.add_at_least_k(lits, k)')
            writer.gen_return_py_none()
        self.add_method('add_at_least_k',
                        func_body=meth_add_at_least_k,
                        arg_list=[SatLiteralListArg(name='lits',
                                                    cvarname='lits'),
                                  UlongArg(name='k',
                                           cvarname='k')],
                        doc_str="add clauses representing 'AT LEAST K'")

        def meth_add_exact_one(writer):
            writer.gen_stmt('val.add_exact_one(lits)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_exact_one',
                                    func_body=meth_add_exact_one,
                                    arg_parser=LitsParser(cvarname='lits'),
                                    doc_str="add clauses representing 'AT EXACT ONE'")

        def meth_add_exact_two(writer):
            writer.gen_stmt('val.add_exact_two(lits)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_exact_two',
                                    func_body=meth_add_exact_two,
                                    arg_parser=LitsParser(cvarname='lits'),
                                    doc_str="add clauses representing 'AT EXACT TWO'")

        def meth_add_exact_k(writer):
            writer.gen_stmt('val.add_exact_k(lits, k)')
            writer.gen_return_py_none()
        self.add_method('add_exact_k',
                        func_body=meth_add_exact_k,
                        arg_list=[SatLiteralListArg(name='lits',
                                                    cvarname='lits'),
                                  UlongArg(name='k',
                                           cvarname='k')],
                        doc_str="add clauses representing 'AT EXACT K'")

        def meth_add_not_one(writer):
            writer.gen_stmt('val.add_not_one(lits)')
            writer.gen_return_py_none()
        self.add_method_with_parser('add_not_one',
                                    func_body=meth_add_not_one,
                                    arg_parser=LitsParser(cvarname='lits'),
                                    doc_str="add clauses representing 'AT NOT ONE'")

        def meth_add_eq(writer):
            with writer.gen_if_block('PySatLiteralList::Check(b_obj)'):
                writer.gen_auto_assign('blits', 'PySatLiteralList::Get(b_obj)')
                writer.gen_stmt("val.add_eq(a, blits)")
            with writer.gen_elseif_block('PyInt::Check(b_obj)'):
                writer.gen_auto_assign('bval', 'PyInt::Get(b_obj)')
                writer.gen_stmt('val.add_eq(a, bval)')
            with writer.gen_else_block():
                writer.gen_type_error('"2nd argument should be \'list of SatLiterals\' or \'integer\' "')
            writer.gen_return_py_none()
        self.add_method('add_eq',
                        func_body=meth_add_eq,
                        arg_list=[SatLiteralListArg(name='a',
                                                    cvarname='a'),
                                  RawObjArg(name='b',
                                            cvarname='b_obj')],
                        doc_str="add clauses representing 'EQ'")

        def meth_add_ne(writer):
            with writer.gen_if_block('PySatLiteralList::Check(b_obj)'):
                writer.gen_auto_assign('blits', 'PySatLiteralList::Get(b_obj)')
                writer.gen_stmt("val.add_ne(a, blits)")
            with writer.gen_elseif_block('PyInt::Check(b_obj)'):
                writer.gen_auto_assign('bval', 'PyInt::Get(b_obj)')
                writer.gen_stmt('val.add_ne(a, bval)')
            with writer.gen_else_block():
                writer.gen_type_error('"2nd argument should be \'list of SatLiterals\' or \'integer\' "')
            writer.gen_return_py_none()
        self.add_method('add_ne',
                        func_body=meth_add_ne,
                        arg_list=[SatLiteralListArg(name='a',
                                                    cvarname='a'),
                                  RawObjArg(name='b',
                                            cvarname='b_obj')],
                        doc_str="add clauses representing 'NE'")

        def meth_add_lt(writer):
            with writer.gen_if_block('PySatLiteralList::Check(b_obj)'):
                writer.gen_auto_assign('blits', 'PySatLiteralList::Get(b_obj)')
                writer.gen_stmt("val.add_lt(a, blits)")
            with writer.gen_elseif_block('PyInt::Check(b_obj)'):
                writer.gen_auto_assign('bval', 'PyInt::Get(b_obj)')
                writer.gen_stmt('val.add_lt(a, bval)')
            with writer.gen_else_block():
                writer.gen_type_error('"2nd argument should be \'list of SatLiterals\' or \'integer\' "')
            writer.gen_return_py_none()
        self.add_method('add_lt',
                        func_body=meth_add_lt,
                        arg_list=[SatLiteralListArg(name='a',
                                                    cvarname='a'),
                                  RawObjArg(name='b',
                                            cvarname='b_obj')],
                        doc_str="add clauses representing 'LT'")

        def meth_add_le(writer):
            with writer.gen_if_block('PySatLiteralList::Check(b_obj)'):
                writer.gen_auto_assign('blits', 'PySatLiteralList::Get(b_obj)')
                writer.gen_stmt("val.add_le(a, blits)")
            with writer.gen_elseif_block('PyInt::Check(b_obj)'):
                writer.gen_auto_assign('bval', 'PyInt::Get(b_obj)')
                writer.gen_stmt('val.add_le(a, bval)')
            with writer.gen_else_block():
                writer.gen_type_error('"2nd argument should be \'list of SatLiterals\' or \'integer\' "')
            writer.gen_return_py_none()
        self.add_method('add_le',
                        func_body=meth_add_le,
                        arg_list=[SatLiteralListArg(name='a',
                                                    cvarname='a'),
                                  RawObjArg(name='b',
                                            cvarname='b_obj')],
                        doc_str="add clauses representing 'LE'")

        def meth_add_gt(writer):
            with writer.gen_if_block('PySatLiteralList::Check(b_obj)'):
                writer.gen_auto_assign('blits', 'PySatLiteralList::Get(b_obj)')
                writer.gen_stmt("val.add_gt(a, blits)")
            with writer.gen_elseif_block('PyInt::Check(b_obj)'):
                writer.gen_auto_assign('bval', 'PyInt::Get(b_obj)')
                writer.gen_stmt('val.add_gt(a, bval)')
            with writer.gen_else_block():
                writer.gen_type_error('"2nd argument should be \'list of SatLiterals\' or \'integer\' "')
            writer.gen_return_py_none()
        self.add_method('add_gt',
                        func_body=meth_add_gt,
                        arg_list=[SatLiteralListArg(name='a',
                                                    cvarname='a'),
                                  RawObjArg(name='b',
                                            cvarname='b_obj')],
                        doc_str="add clauses representing 'GT'")

        def meth_add_ge(writer):
            with writer.gen_if_block('PySatLiteralList::Check(b_obj)'):
                writer.gen_auto_assign('blits', 'PySatLiteralList::Get(b_obj)')
                writer.gen_stmt("val.add_ge(a, blits)")
            with writer.gen_elseif_block('PyInt::Check(b_obj)'):
                writer.gen_auto_assign('bval', 'PyInt::Get(b_obj)')
                writer.gen_stmt('val.add_ge(a, bval)')
            with writer.gen_else_block():
                writer.gen_type_error('"2nd argument should be \'list of SatLiterals\' or \'integer\' "')
            writer.gen_return_py_none()
        self.add_method('add_ge',
                        func_body=meth_add_ge,
                        arg_list=[SatLiteralListArg(name='a',
                                                    cvarname='a'),
                                  RawObjArg(name='b',
                                            cvarname='b_obj')],
                        doc_str="add clauses representing 'GE'")

        def meth_condblock(writer):
            writer.gen_return_pyobject('PySatCondBlock',
                                       'val.condblock(lits_list)')
        self.add_method_with_parser('condblock',
                                    func_body=meth_condblock,
                                    arg_parser=LitsParser(cvarname='lits_list'),
                                    doc_str='make SatCondBlock')

        def meth_solve(writer):
            writer.gen_vardecl(typename='std::vector<SatLiteral>',
                               varname='assumptions')
            with writer.gen_if_block('obj1 != nullptr'):
                with writer.gen_if_block('PySatLiteral::Check(obj1)'):
                    writer.gen_autoref_assign('lit', 'PySatLiteral::_get_ref(obj1)')
                    writer.gen_stmt('assumptions.push_back(lit)')
                with writer.gen_elseif_block('PySatLiteralList::Check(obj1)'):
                    writer.gen_assign('assumptions',
                                      'PySatLiteralList::Get(obj1)')
                with writer.gen_else_block():
                    writer.gen_type_error('"\'SatLiteral\' or sequence of \'SatLiteral\' is expected"')
            writer.gen_auto_assign('ans', 'val.solve(assumptions, time_limit)')
            writer.gen_return_pyobject('PySatBool3', 'ans')
        self.add_method('solve',
                        func_body=meth_solve,
                        arg_list=[OptArg(),
                                  RawObjArg(name='assumptions',
                                            cvarname='obj1'),
                                  KwdArg(),
                                  IntArg(name='time_limit',
                                         cvarname='time_limit',
                                         cvardefault=0)],
                        doc_str="solve SAT problem")

        def meth_model(writer):
            writer.gen_autoref_assign('model', 'val.model()')
            writer.gen_return_pyobject('PySatModel', 'model')
        self.add_method('model',
                        func_body=meth_model,
                        doc_str="return the model for SAT problem")

        def meth_read_model(writer):
            writer.gen_autoref_assign('model', 'val.model()')
            writer.gen_auto_assign('ans', 'model[lit]')
            writer.gen_return_pyobject('PySatBool3', 'ans')
        self.add_method('read_model',
                        func_body=meth_read_model,
                        arg_list=[SatLiteralArg(name='lit',
                                                cvarname='lit')],
                        doc_str="read model value for specified literal")

        def meth_conflict_literal(writer):
            writer.gen_autoref_assign('lits', 'val.conflict_literals()')
            writer.gen_return_pyobject('PySatLiteralList', 'lits')
        self.add_method('conflict_literals',
                        func_body=meth_conflict_literal,
                        doc_str="return conflict literals of the last SAT problem")


if __name__ == '__main__':

    gen = SatSolverGen()

    gen.make_header()

    gen.make_source()
