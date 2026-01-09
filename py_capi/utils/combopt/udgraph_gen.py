#! /usr/bin/env python3

""" PyUdGraph を生成するスクリプト

:file: udgraph_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import UlongArg, StringArg
from mk_py_capi import OptArg, KwdArg
from misc import JsonValueArg


class UdGraphGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='UdGraph',
                         pyname='UdGraph',
                         namespace='YM',
                         header_include_files=['ym/UdGraph.h'],
                         source_include_files=['pym/PyUdGraph.h',
                                               'pym/PyUdEdge.h',
                                               'pym/PyUdEdgeList.h',
                                               'pym/PyJsonValue.h',
                                               'pym/PyUlong.h'])

        def new_func(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.gen_stmt('new (&my_obj->mVal) UdGraph(node_num)')
            writer.gen_return_self()
        self.add_new(func_body=new_func,
                     arg_list=[OptArg(),
                               KwdArg(),
                               UlongArg(name='node_num',
                                        cvarname='node_num',
                                        cvardefault=0)])

        self.add_dealloc(None)

        def meth_resize(writer):
            writer.gen_stmt('val.resize(node_num)')
            writer.gen_return_py_none()
        self.add_method('resize',
                        func_body=meth_resize,
                        arg_list=[UlongArg(name='node_num',
                                           cvarname='node_num')],
                        doc_str='resize')

        def meth_connect(writer):
            writer.gen_stmt('val.connect(id1, id2)')
            writer.gen_return_py_none()
        self.add_method('connect',
                        func_body=meth_connect,
                        arg_list=[UlongArg(name='id1',
                                           cvarname='id1'),
                                  UlongArg(name='id2',
                                           cvarname='id2')],
                        doc_str='connect')

        def get_node_num(writer):
            writer.gen_return_py_ulong('val.node_num()')
        self.add_getter('get_node_num',
                        func_body=get_node_num)
        self.add_attr('node_num',
                      getter_name='get_node_num')

        def get_edge_num(writer):
            writer.gen_return_py_ulong('val.edge_num()')
        self.add_getter('get_edge_num',
                        func_body=get_edge_num)
        self.add_attr('edge_num',
                      getter_name='get_edge_num')

        def meth_is_reflective(writer):
            writer.gen_return_py_bool('val.is_reflective()')
        self.add_method('is_reflective',
                        func_body=meth_is_reflective,
                        doc_str='True if reflective graph')

        def meth_edge(writer):
            writer.gen_return_pyobject('PyUdEdge',
                                       'val.edge(id)')
        self.add_method('edge',
                        func_body=meth_edge,
                        arg_list=[UlongArg(name='id',
                                           cvarname='id')],
                        doc_str='return Edge')

        def get_edge_list(writer):
            writer.gen_return_pyobject('PyUdEdgeList',
                                       'val.edge_list()')
        self.add_getter('get_edge_list',
                        func_body=get_edge_list)
        self.add_attr('edge_list',
                      getter_name='get_edge_list')

        def meth_read_dimacs(writer):
            writer.gen_return_pyobject('PyUdGraph',
                                       'UdGraph::read_dimacs(filename)')
        self.add_static_method('read_dimacs',
                               func_body=meth_read_dimacs,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               doc_str='read DIMACS format')

        def meth_write_dimacs(writer):
            writer.gen_stmt('val.write_dimacs(filename)')
            writer.gen_return_py_none()
        self.add_method('write_dimacs',
                        func_body=meth_write_dimacs,
                        arg_list=[StringArg(name='filename',
                                            cvarname='filename')],
                        doc_str='write in DIMCAS format')

        def meth_coloring(writer):
            writer.gen_vardecl(typename='std::vector<SizeType>',
                               varname='color_map')
            writer.gen_auto_assign('nc', 'val.coloring(color_map, option)')
            writer.gen_auto_assign('cmap_obj',
                                   'PyList<SizeType, PyUlong>::ToPyObject(color_map)')
            writer.gen_return_buildvalue('(Ok)', ['cmap_obj', 'nc'])
        self.add_method('coloring',
                        func_body=meth_coloring,
                        arg_list=[OptArg(),
                                  KwdArg(),
                                  JsonValueArg(name='option',
                                               cvarname='option')],
                        doc_str='do minimum coloring')

        """
        def meth_independent_set(writer):
            writer.gen_return_pyobject('PyList<SizeType, PyUlong>',
                                       'val.independent_set(option)')
        self.add_method('independent_set',
                        func_body=meth_independent_set,
                        arg_list=[OptArg(),
                                  KwdArg(),
                                  JsonValueArg(name='option',
                                               cvarname='option')],
                        doc_str='get independent set')
        """

        def meth_max_clique(writer):
            writer.gen_return_pyobject('PyList<SizeType, PyUlong>',
                                       'val.max_clique(option)')
        self.add_method('max_clique',
                        func_body=meth_max_clique,
                        arg_list=[OptArg(),
                                  KwdArg(),
                                  JsonValueArg(name='option',
                                               cvarname='option')],
                        doc_str='get max clique')

        self.add_conv('default')
        self.add_deconv('default')
