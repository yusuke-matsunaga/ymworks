#! /usr/bin/env python3

""" PyBnNodeList を生成するスクリプト

:file: bnnodelist_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, RawObjArg


class BnNodeListGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BnNodeList',
                         pyname='BnNodeList',
                         namespace='YM',
                         header_include_files=['ym/BnNodeList.h'],
                         source_include_files=['pym/PyBnNodeList.h',
                                               'pym/PyBnNode.h',
                                               'pym/PyBnNodeIter2.h',
                                               'pym/PyList.h',
                                               'pym/PyUlong.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            with writer.gen_if_block('obj != nullptr'):
                writer.gen_vardecl(typename='BnNodeList',
                                   varname='node_list')
                with writer.gen_if_block('!PyBnNodeList::FromPyObject(obj, node_list)'):
                    writer.gen_type_error('"argument 1 must be a BnNodeList or a list of BnNode"')
                writer.gen_stmt('new (&my_obj->mVal) BnNodeList(node_list)')
            with writer.gen_else_block():
                writer.gen_stmt('new (&my_obj->mVal) BnNodeList()')
            writer.gen_return_self()
        self.add_new(new_body,
                     arg_list=[OptArg(),
                               RawObjArg(cvarname='obj',
                                         cvardefault='nullptr')])

        self.add_dealloc('default')

        def sq_length(writer):
            writer.gen_auto_assign('len_val', 'val.size()')
        def sq_item(writer):
            writer.gen_return_pyobject('PyBnNode',
                                       'val[index]')
        self.add_sequence(sq_length=sq_length,
                          sq_item=sq_item)

        def iter_func(writer):
            writer.gen_return_pyobject('PyBnNodeIter2',
                                       'val.iter()')
        self.add_iter(iter_func)

        self.add_conv('default')

        def deconv_func(writer):
            self.gen_raw_conv(writer)
            with writer.gen_block():
                writer.gen_vardecl(typename='std::vector<BnNode>',
                                   varname='node_list')
                with writer.gen_if_block('PyList<BnNode, PyBnNode>::FromPyObject(obj, node_list)'):
                    writer.gen_assign('val', 'BnNodeList(node_list)')
                    writer.gen_return('true')
            writer.gen_return('false')
        self.add_deconv(deconv_func)
