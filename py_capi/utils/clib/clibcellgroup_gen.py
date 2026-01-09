#! /usr/bin/env python3

""" PyClibCellGroup を生成するスクリプト

:file: clibcellgroup_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import LongArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg
from .clib_arg import ClibTimingSenseArg


class ClibCellGroupGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibCellGroup',
                         pyname='ClibCellGroup',
                         namespace='YM',
                         header_include_files=['ym/ClibCellGroup.h'],
                         source_include_files=['pym/PyClibCellGroup.h',
                                               'pym/PyClibCellClass.h',
                                               'pym/PyClibCell.h',
                                               'pym/PyClibCellType.h',
                                               'pym/PyClibIOMap.h',
                                               'pym/PyClibSeqAttr.h',
                                               'pym/PyClibList.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc('default')

        def get_rep_class(writer):
            writer.gen_return_pyobject('PyClibCellClass',
                                       'val.rep_class()')
        self.add_getter('get_rep_class',
                        func_body=get_rep_class)
        self.add_attr('rep_class',
                      getter_name='get_rep_class',
                      doc_str='representative class')

        def get_iomap(writer):
            writer.gen_return_pyobject('PyClibIOMap',
                                       'val.iomap()')
        self.add_getter('get_iomap',
                        func_body=get_iomap)
        self.add_attr('iomap',
                      getter_name='get_iomap',
                      doc_str='IO map for representative class')

        def get_cell_type(writer):
            writer.gen_return_pyobject('PyClibCellType',
                                       'val.cell_type()')
        self.add_getter('get_cell_type',
                        func_body=get_cell_type)
        self.add_attr('cell_type',
                      getter_name='get_cell_type',
                      doc_str='cell type(Logic, FF, Latch, FSM)')

        def get_seq_attr(writer):
            writer.gen_return_pyobject('PyClibSeqAttr',
                                       'val.seq_attr()')
        self.add_getter('get_seq_attr',
                        func_body=get_seq_attr)
        self.add_attr('seq_attr',
                      getter_name='get_seq_attr',
                      doc_str='ClibSeqAttr')

        def get_cell_num(writer):
            writer.gen_return_py_ulong('val.cell_num()')
        self.add_getter('get_cell_num',
                        func_body=get_cell_num)
        self.add_attr('cell_num',
                      getter_name='get_cell_num',
                      doc_str='number of cells')

        def meth_cell(writer):
            writer.gen_return_pyobject('PyClibCell',
                                       'val.cell(pos)')
        self.add_method('cell',
                        func_body=meth_cell,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return cell')

        def meth_cell_list(writer):
            writer.gen_return_pyobject('PyClibCellList',
                                       'val.cell_list()')
        self.add_method('meth_cell_list',
                        func_body=meth_cell_list,
                        doc_str='return list of cells')

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
