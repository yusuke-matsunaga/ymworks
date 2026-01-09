#! /usr/bin/env python3

""" PyClibCellLibrary を生成するスクリプト

:file: clibiomap_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import LongArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg


class ClibIOMapGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibIOMap',
                         pyname='ClibIOMap',
                         namespace='YM',
                         header_include_files=['ym/ClibIOMap.h'],
                         source_include_files=['pym/PyClibIOMap.h',
                                               'pym/PyClibPinMap.h',
                                               'pym/PyUlong.h',
                                               'pym/PyList.h'])

        self.add_dealloc('default')

        def meth_identity(writer):
            writer.gen_return_pyobject('PyClibIOMap',
                                       'ClibIOMap::identity(ni, no, nb)')
        self.add_static_method('identity',
                               func_body=meth_identity,
                               arg_list=[UlongArg(name='ni',
                                                  cvarname='ni'),
                                         OptArg(),
                                         UlongArg(name='no',
                                                  cvarname='no',
                                                  cvardefault=1),
                                         UlongArg(name='nb',
                                                  cvarname='nb',
                                                  cvardefault=0)],
                               doc_str='make IDENTITY map')

        def get_input_num(writer):
            writer.gen_return_py_ulong('val.input_num()')
        self.add_getter('get_input_num',
                        func_body=get_input_num)
        self.add_attr('input_num',
                      getter_name='get_input_num',
                      doc_str='number of inputs')

        def meth_input_map(writer):
            writer.gen_return_pyobject('PyClibPinMap',
                                       'val.input_map(pos)')
        self.add_method('input_map',
                        func_body=meth_input_map,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return input map')

        def meth_input_map_list(writer):
            writer.gen_return_pyobject('PyList<ClibPinMap, PyClibPinMap>',
                                       'val.input_map_list()')
        self.add_method('meth_input_map_list',
                        func_body=meth_input_map_list,
                        doc_str='return list of input maps')

        def get_output_num(writer):
            writer.gen_return_py_ulong('val.output_num()')
        self.add_getter('get_output_num',
                        func_body=get_output_num)
        self.add_attr('output_num',
                      getter_name='get_output_num',
                      doc_str='number of outputs')

        def meth_output_map(writer):
            writer.gen_return_pyobject('PyClibPinMap',
                                       'val.output_map(pos)')
        self.add_method('output_map',
                        func_body=meth_output_map,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return output map')

        def meth_output_map_list(writer):
            writer.gen_return_pyobject('PyList<ClibPinMap, PyClibPinMap>',
                                       'val.output_map_list()')
        self.add_method('meth_output_map_list',
                        func_body=meth_output_map_list,
                        doc_str='return list of output maps')

        def get_inout_num(writer):
            writer.gen_return_py_ulong('val.inout_num()')
        self.add_getter('get_inout_num',
                        func_body=get_inout_num)
        self.add_attr('inout_num',
                      getter_name='get_inout_num',
                      doc_str='number inouts')

        def meth_inout_map(writer):
            writer.gen_return_pyobject('PyClibPinMap',
                                       'val.inout_map(pos)')
        self.add_method('inout_map',
                        func_body=meth_inout_map,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return inout map')

        def meth_inout_map_list(writer):
            writer.gen_return_pyobject('PyList<ClibPinMap, PyClibPinMap>',
                                       'val.inout_map_list()')
        self.add_method('meth_inout_map_list',
                        func_body=meth_inout_map_list,
                        doc_str='return list of inout maps')

        self.add_nb_multiply()
        self.add_nb_inplace_multiply()

        def meth_inverse(writer):
            writer.gen_return_pyobject('PyClibIOMap',
                                       'val.inverse()')
        self.add_method('inverse',
                        func_body=meth_inverse,
                        doc_str='INVERSE operation')

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
