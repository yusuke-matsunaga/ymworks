#! /usr/bin/env python3

""" PyFraigMgr を生成するスクリプト

:file: fraigmgr_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg
from sat import SatInitParamArg, SatBool3Arg
from .fraig_arg import FraigLiteralArg, FraigLiteralListArg

class FraigMgrGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='FraigMgr',
                         pyname='FraigMgr',
                         namespace='YM',
                         header_include_files=['ym/FraigMgr.h'],
                         source_include_files=['pym/PyFraigMgr.h',
                                               'pym/PyFraigLiteral.h',
                                               'pym/PySatBool3.h',
                                               'pym/PySatInitParam.h',
                                               'pym/PyUlong.h',
                                               'pym/PyList.h'])

        def new_func(writer):
            self.gen_alloc_code(writer, varname='self')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.gen_stmt('new (&my_obj->mVal) FraigMgr(sig_size, init_param)')
            writer.gen_return_self()
        self.add_new(func_body=new_func,
                     arg_list=[KwdArg(),
                               UlongArg(name='sig_size',
                                        cvarname='sig_size'),
                               OptArg(),
                               SatInitParamArg(name='init_param',
                                               cvarname='init_param')])
        self.add_dealloc('default')

        def meth_new_input(writer):
            writer.gen_return_pyobject('PyFraigLiteral',
                                       'val.new_input()')
        self.add_method('new_input',
                        func_body=meth_new_input,
                        doc_str='make new Input')

        def meth_new_and(writer):
            writer.gen_return_pyobject('PyFraigLiteral',
                                       'val.new_and(fanin_list)')
        self.add_method('new_and',
                        func_body=meth_new_and,
                        arg_list=[FraigLiteralListArg(name='fanin_list',
                                                      cvarname='fanin_list')],
                        doc_str='make new AND')

        def meth_new_or(writer):
            writer.gen_return_pyobject('PyFraigLiteral',
                                       'val.new_or(fanin_list)')
        self.add_method('new_or',
                        func_body=meth_new_or,
                        arg_list=[FraigLiteralListArg(name='fanin_list',
                                                      cvarname='fanin_list')],
                        doc_str='make new OR')

        def meth_new_xor(writer):
            writer.gen_return_pyobject('PyFraigLiteral',
                                       'val.new_xor(fanin_list)')
        self.add_method('new_xor',
                        func_body=meth_new_xor,
                        arg_list=[FraigLiteralListArg(name='fanin_list',
                                                      cvarname='fanin_list')],
                        doc_str='make new XOR')

        def meth_check_equiv(writer):
            writer.gen_return_pyobject('PySatBool3',
                                       'val.check_equiv(lit1, lit2)')
        self.add_method('check_equiv',
                        func_body=meth_check_equiv,
                        arg_list=[FraigLiteralArg(name='lit1',
                                                  cvarname='lit1'),
                                  FraigLiteralArg(name='lit2',
                                                  cvarname='lit2')],
                        doc_str='do EQUIVALENCE CHECKING')

        def meth_set_loglevel(writer):
            writer.gen_stmt('val.set_loglevel(level)')
            writer.gen_return_py_none()
        self.add_method('set_loglevel',
                        func_body=meth_set_loglevel,
                        arg_list=[UlongArg(name='level',
                                           cvarname='level')],
                        doc_str='set log level')
