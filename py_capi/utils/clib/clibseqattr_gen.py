#! /usr/bin/env python3

""" PyClibSeqAttr を生成するスクリプト

:file: clibseqattr_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import LongArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg


class ClibSeqAttrGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibSeqAttr',
                         pyname='ClibSeqAttr',
                         namespace='YM',
                         header_include_files=['ym/ClibSeqAttr.h'],
                         source_include_files=['pym/PyClibSeqAttr.h',
                                               'pym/PyClibCPV.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc('default')

        def meth_has_slave_clock(writer):
            writer.gen_return_py_bool('val.has_slave_clock()')
        self.add_method('has_slave_clock',
                        func_body=meth_has_slave_clock,
                        doc_str='True if having slave clock')

        def meth_has_clear(writer):
            writer.gen_return_py_bool('val.has_clear()')
        self.add_method('has_clear',
                        func_body=meth_has_clear,
                        doc_str='True if having clear pin')

        def meth_has_preset(writer):
            writer.gen_return_py_bool('val.has_preset()')
        self.add_method('has_preset',
                        func_body=meth_has_preset,
                        doc_str='True if having preset pin')

        def get_cpv1(writer):
            writer.gen_return_pyobject('PyClibCPV',
                                       'val.cpv1()')
        self.add_getter('get_cpv1',
                        func_body=get_cpv1)
        self.add_attr('cpv1',
                      getter_name='get_cpv1',
                      doc_str='clear preset var1')

        def get_cpv2(writer):
            writer.gen_return_pyobject('PyClibCPV',
                                       'val.cpv2()')
        self.add_getter('get_cpv2',
                        func_body=get_cpv2)
        self.add_attr('cpv2',
                      getter_name='get_cpv2',
                      doc_str='clear preset var2')

        def get_index(writer):
            writer.gen_return_py_ulong('val.index()')
        self.add_getter('get_index',
                        func_body=get_index)
        self.add_attr('index',
                      getter_name='get_index',
                      doc_str='index')

        def meth_max_index(writer):
            writer.gen_return_py_ulong('ClibSeqAttr::max_index()')
        self.add_static_method('max_index',
                               func_body=meth_max_index,
                               doc_str='return MAX INDEX')

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
