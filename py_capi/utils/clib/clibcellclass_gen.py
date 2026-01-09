#! /usr/bin/env python3

""" PyClibCellClass を生成するスクリプト

:file: clibcellclass_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import LongArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg
from .clib_arg import ClibTimingSenseArg


class ClibCellClassGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibCellClass',
                         pyname='ClibCellClass',
                         namespace='YM',
                         header_include_files=['ym/ClibCellClass.h'],
                         source_include_files=['pym/PyClibCellClass.h',
                                               'pym/PyClibCellGroup.h',
                                               'pym/PyClibIOMap.h',
                                               'pym/PyClibList.h',
                                               'pym/PyList.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc('default')

        def get_idmap_num(writer):
            writer.gen_return_py_ulong('val.idmap_num()')
        self.add_getter('get_idmap_num',
                        func_body=get_idmap_num)
        self.add_attr('idmap_num',
                      getter_name='get_idmap_num',
                      doc_str='number of idmaps')

        def meth_idmap(writer):
            writer.gen_return_pyobject('PyClibIOMap',
                                       'val.idmap(pos)')
        self.add_method('idmap',
                        func_body=meth_idmap,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return idmap')

        def meth_idmap_list(writer):
            writer.gen_return_pyobject('PyList<ClibIOMap, PyClibIOMap>',
                                       'val.idmap_list()')
        self.add_method('meth_idmap_list',
                        func_body=meth_idmap_list,
                        doc_str='return list of idmaps')

        def get_cell_group_num(writer):
            writer.gen_return_py_ulong('val.cell_group_num()')
        self.add_getter('get_cell_group_num',
                        func_body=get_cell_group_num)
        self.add_attr('cell_group_num',
                      getter_name='get_cell_group_num',
                      doc_str='number of CellGroups')

        def meth_cell_group(writer):
            writer.gen_return_pyobject('PyClibCellGroup',
                                       'val.cell_group(pos)')
        self.add_method('cell_group',
                        func_body=meth_cell_group,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return CellGroup')

        def meth_cell_group_list(writer):
            writer.gen_return_pyobject('PyClibCellGroupList',
                                       'val.cell_group_list()')
        self.add_method('meth_cell_group_list',
                        func_body=meth_cell_group_list,
                        doc_str='return list of CellGroups')

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

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
