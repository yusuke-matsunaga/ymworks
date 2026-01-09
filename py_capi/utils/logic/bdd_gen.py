#! /usr/bin/env python3

""" PyBdd を生成するスクリプト

:file: bdd_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.

**TODO**
2025/08/25:
bdd_size() の引数を可変引数で BddArg か BddListArg を受け付けるようにする．
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import MulOp, MulIop, DivOp, DivIop
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg
from .logic_arg import BddArg, BddListArg, BddVarArg, BddVarListArg
from .logic_arg import BddCompMapArg, BddVarMapArg


class BddGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='Bdd',
                         pyname='Bdd',
                         namespace='YM',
                         header_include_files=['ym/Bdd.h'],
                         source_include_files=['pym/PyBdd.h',
                                               'pym/PyBddMgr.h',
                                               'pym/PyBddVar.h',
                                               'pym/PyBddLit.h',
                                               'pym/PyBddCube.h',
                                               'pym/PyBddCompMap.h',
                                               'pym/PyBddVarMap.h',
                                               'pym/PyBddVarSet.h',
                                               'pym/PyLiteral.h',
                                               'pym/PyNpnMap.h',
                                               'pym/PyInt.h',
                                               'pym/PyLong.h',
                                               'pym/PyBool.h',
                                               'pym/PyString.h',
                                               'pym/PyList.h',
                                               'pym/PyModule.h',
                                               'ym/BddVar.h',
                                               'ym/BddLit.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) Bdd();')
            writer.gen_return_self()
        self.add_new(new_body)

        self.add_dealloc('default')

        """
        def repr_func(writer):
            writer.gen_auto_assign('str_val', 'val.str()')
        self.add_repr(func_body=repr_func)
        """

        def meth_zero(writer):
            writer.gen_return_pyobject('PyBdd', 'Bdd::zero()')
        self.add_static_method('zero',
                               func_body=meth_zero,
                               doc_str='make ZERO Bdd object')

        def meth_one(writer):
            writer.gen_return_pyobject('PyBdd', 'Bdd::one()')
        self.add_static_method('one',
                               func_body=meth_one,
                               doc_str='make ONE Bdd object')

        def meth_invalid(writer):
            writer.gen_return_pyobject('PyBdd', 'Bdd::invalid()')
        self.add_static_method('invalid',
                               func_body=meth_invalid,
                               doc_str='make invalid Bdd object')

        def meth_cofactor(writer):
            with writer.gen_if_block('PyBddVar::Check(arg_obj)'):
                writer.gen_autoref_assign('var', 'PyBddVar::_get_ref(arg_obj)')
                writer.gen_return_pyobject('PyBdd',
                                           'val.cofactor(var, inv)')
            with writer.gen_if_block('PyBddLit::Check(arg_obj)'):
                writer.gen_autoref_assign('lit', 'PyBddLit::_get_ref(arg_obj)')
                with writer.gen_if_block('inv'):
                    writer.gen_assign('lit', '~lit')
                writer.gen_return_pyobject('PyBdd',
                                           'val.cofactor(lit)')
            with writer.gen_if_block('PyBddCube::Check(arg_obj)'):
                with writer.gen_if_block('inv'):
                    writer.gen_type_error('"inv should not be specified with BddCube"')
                writer.gen_autoref_assign('cube', 'PyBddCube::_get_ref(arg_obj)')
                writer.gen_return_pyobject('PyBdd',
                                           'val.cofactor(cube)')
            writer.gen_type_error('"argument 1 should be \'BddVar\', \'BddLit\', or \'BddCube\'"')
        self.add_method('cofactor',
                        func_body=meth_cofactor,
                        arg_list=[RawObjArg(cvarname='arg_obj'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv',
                                          cvardefault='false')],
                        doc_str='make cofactor')

        def meth_ite(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'ite(val1, val2, val3)')
        self.add_static_method('ite',
                               func_body=meth_ite,
                               arg_list=[BddArg(name='cond_bdd',
                                                cvarname='val1'),
                                         BddArg(name='then_bdd',
                                                cvarname='val2'),
                                         BddArg(name='else_bdd',
                                                cvarname='val3')],
                               doc_str='ITE op')

        def meth_simplify(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'simplify(on, dc)')
        self.add_static_method('simplify',
                               func_body=meth_simplify,
                               arg_list=[BddArg(name='on',
                                                cvarname='on'),
                                         BddArg(name='dc',
                                                cvarname='dc')],
                               doc_str='do simplify with DontCare')

        def meth_compose(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'val.compose(var, operand)')
        self.add_method('compose',
                        func_body=meth_compose,
                        arg_list=[BddVarArg(name='var',
                                            cvarname='var'),
                                  BddArg(name='operand',
                                         cvarname='operand')],
                        doc_str='COMPOSE op')

        def meth_multi_compose(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'val.multi_compose(compose_map)')
        self.add_method('multi_compose',
                        func_body=meth_multi_compose,
                        arg_list=[BddCompMapArg(name='compose_map',
                                                cvarname='compose_map')],
                        doc_str='multiway COMPOSE op')

        def meth_remap_vars(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'val.remap_vars(var_map)')
        self.add_method('remap_vars',
                        func_body=meth_remap_vars,
                        arg_list=[BddVarMapArg(name='var_map',
                                               cvarname='var_map')],
                        doc_str='remap variable ordering')

        def meth_is_invalid(writer):
            writer.gen_return_py_bool('val.is_invalid()')
        self.add_method('is_invalid',
                        func_body=meth_is_invalid,
                        doc_str='return True if invalid')

        def meth_is_zero(writer):
            writer.gen_return_py_bool('val.is_zero()')
        self.add_method('is_zero',
                        func_body=meth_is_zero,
                        doc_str='return True if ZERO')

        def meth_is_one(writer):
            writer.gen_return_py_bool('val.is_one()')
        self.add_method('is_one',
                        func_body=meth_is_one,
                        doc_str='return True if ONE')

        def meth_is_const(writer):
            writer.gen_return_py_bool('val.is_const()')
        self.add_method('is_const',
                        func_body=meth_is_const,
                        doc_str='return True if constant(ZERO or ONE)')

        def meth_check_sup(writer):
            writer.gen_return_py_bool('val.check_sup(var)')
        self.add_method('check_sup',
                        func_body=meth_check_sup,
                        arg_list=[BddVarArg(name='var',
                                            cvarname='var')],
                        doc_str='check support variable')

        def meth_check_sym(writer):
            writer.gen_return_py_bool('val.check_sym(var1, var2, inv)')
        self.add_method('check_sym',
                        func_body=meth_check_sym,
                        arg_list=[BddVarArg(name='var1',
                                            cvarname='var1'),
                                  BddVarArg(name='var2',
                                            cvarname='var2'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv',
                                          cvardefault='false')],
                        doc_str='check symmmetry')

        def meth_get_support(writer):
            writer.gen_return_pyobject('PyBddVarSet',
                                       'val.get_support()')
        self.add_method('get_support',
                        func_body=meth_get_support,
                        doc_str='get support variable')

        def meth_get_onepath(writer):
            writer.gen_return_pyobject('PyBddCube',
                                       'val.get_onepath()')
        self.add_method('get_onepath',
                        func_body=meth_get_onepath,
                        doc_str='get ONE path')

        def meth_get_zeropath(writer):
            writer.gen_return_pyobject('PyBddCube',
                                       'val.get_zeropath()')
        self.add_method('get_zeropath',
                        func_body=meth_get_zeropath,
                        doc_str='get ZERO path')

        def meth_root_decomp(writer):
            writer.gen_vardecl(typename='Bdd',
                               varname='f0')
            writer.gen_vardecl(typename='Bdd',
                               varname='f1')
            writer.gen_auto_assign('var',
                                   'val.root_decomp(f0, f1)')
            writer.gen_auto_assign('var_obj', 'PyBddVar::ToPyObject(var)')
            writer.gen_auto_assign('f0_obj', 'PyBdd::ToPyObject(f0)')
            writer.gen_auto_assign('f1_obj', 'PyBdd::ToPyObject(f1)')
            writer.gen_return_buildvalue('OOO', ['var_obj', 'f0_obj', 'f1_obj'])
        self.add_method('root_decomp',
                        func_body=meth_root_decomp,
                        doc_str='decompose root node')

        def meth_root_var(writer):
            writer.gen_return_pyobject('PyBddVar', 'val.root_var()')
        self.add_method('root_var',
                        func_body=meth_root_var,
                        doc_str='return root variable')

        def meth_root_cofactor0(writer):
            writer.gen_return_pyobject('PyBdd', 'val.root_cofactor0()')
        self.add_method('root_cofactor0',
                        func_body=meth_root_cofactor0,
                        doc_str='return negative cofactor of root variable')

        def meth_root_cofactor1(writer):
            writer.gen_return_pyobject('PyBdd', 'val.root_cofactor1()')
        self.add_method('root_cofactor1',
                        func_body=meth_root_cofactor1,
                        doc_str='return positive cofactor of root variable')

        def meth_eval(writer):
            writer.gen_vardecl(typename='std::vector<bool>',
                               varname='input_vector')
            with writer.gen_if_block('!PyList<bool, PyBool>::FromPyObject(vector_obj, input_vector)'):
                writer.gen_type_error('"artument 1 should be a sequence of bool"')
            writer.gen_return_py_bool('val.eval(input_vector)')
        self.add_method('eval',
                        func_body=meth_eval,
                        arg_list=[RawObjArg(name='input_vector',
                                            cvarname='vector_obj')],
                        doc_str='evaluate input vector')

        def meth_to_truth(writer):
            writer.gen_return_py_string('val.to_truth(var_list)')
        self.add_method('to_truth',
                        func_body=meth_to_truth,
                        arg_list=[BddVarListArg(name='var_list',
                                                cvarname='var_list')],
                        doc_str='convert to truth table')

        def meth_gen_dot(writer):
            pass

        def meth_gen_dot2(writer):
            pass

        def meth_bdd_size(writer):
            writer.gen_return_py_long('bdd_size(bdd_list)')
        self.add_static_method('bdd_size',
                               func_body=meth_bdd_size,
                               arg_list=[BddListArg(name='bdd_list',
                                                    cvarname='bdd_list')],
                               doc_str='return node size of the BDDs')

        def get_size(writer):
            writer.gen_return_py_long('val.size()')
        self.add_getter('get_size',
                        func_body=get_size)
        self.add_attr('size',
                      getter_name='get_size',
                      doc_str='size')

        def get_mgr(writer):
            writer.gen_return_pyobject('PyBddMgr', 'val.mgr()')
        self.add_getter('get_mgr',
                        func_body=get_mgr)
        self.add_attr('mgr',
                      getter_name='get_mgr',
                      doc_str='BddMgr')

        def get_root_inv(writer):
            writer.gen_return_py_bool('val.root_inv()')
        self.add_getter('get_root_inv',
                        func_body=get_root_inv)
        self.add_attr('root_inv',
                      getter_name='get_root_inv',
                      doc_str='True if root edge is inverted')

        self.add_richcompare('eq_default')

        def meth_is_identical(writer):
            writer.gen_return_py_bool('is_identical(val1, val2)')
        self.add_static_method('is_identical',
                               func_body=meth_is_identical,
                               arg_list=[BddArg(name='val1',
                                                cvarname='val1'),
                                         BddArg(name='val2',
                                                cvarname='val2')],
                               doc_str='return True if identical')

        self.add_nb_invert()
        self.add_nb_and()
        self.add_nb_or()
        self.add_nb_xor()
        self.add_nb_inplace_and()
        self.add_nb_inplace_or()
        self.add_nb_inplace_xor()

        self.add_nb_multiply(
            expr=None,
            op_list1=[MulOp('PyBool', useref=False)]
        )

        self.add_nb_inplace_multiply(
            stmt=None,
            op_list1=[MulIop('PyBool', useref=False)]
        )

        self.add_nb_true_divide(
            expr=None,
            op_list1=[DivOp('PyBddLit'),
                      DivOp('PyBddCube')]
        )

        self.add_nb_inplace_true_divide(
            stmt=None,
            op_list1=[DivIop('PyBddLit'),
                      DivIop('PyBddCube')]
        )

        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(hash_func)

        self.add_conv('default')
        self.add_deconv('default')


if __name__ == '__main__':

    gen = BddGen()

    gen.make_header()

    gen.make_source()
