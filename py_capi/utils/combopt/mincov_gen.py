#! /usr/bin/env python3

""" PyMinCov を生成するスクリプト

:file: mincov_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import UlongArg
from mk_py_capi import OptArg, KwdArg
from misc import JsonValueArg


class MinCovGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='MinCov',
                         pyname='MinCov',
                         namespace='YM',
                         header_include_files=['ym/MinCov.h'],
                         source_include_files=['pym/PyMinCov.h',
                                               'pym/PyJsonValue.h',
                                               'pym/PyList.h',
                                               'pym/PyUlong.h'])

        def new_func(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.gen_stmt('new (&my_obj->mVal) MinCov(row_size, col_size)')
            writer.gen_return_self()
        self.add_new(func_body=new_func,
                     arg_list=[OptArg(),
                               KwdArg(),
                               UlongArg(name='row_size',
                                        cvarname='row_size',
                                        cvardefault=0),
                               UlongArg(name='col_size',
                                        cvarname='col_size',
                                        cvardefault=0)])

        self.add_dealloc(None)

        def meth_resize(writer):
            writer.gen_stmt('val.resize(row_size, col_size)')
            writer.gen_return_py_none()
        self.add_method('resize',
                        func_body=meth_resize,
                        arg_list=[KwdArg(),
                                  UlongArg(name='row_size',
                                           cvarname='row_size'),
                                  UlongArg(name='col_size',
                                           cvarname='col_size')],
                        doc_str='resize')

        def meth_set_col_cost(writer):
            writer.gen_stmt('val.set_col_cost(col_pos, cost)')
            writer.gen_return_py_none()
        self.add_method('set_col_cost',
                        func_body=meth_set_col_cost,
                        arg_list=[KwdArg(),
                                  UlongArg(name='col_pos',
                                           cvarname='col_pos'),
                                  UlongArg(name='cost',
                                           cvarname='cost')],
                        doc_str='set column cost')

        def meth_insert_elem(writer):
            writer.gen_stmt('val.insert_elem(row_pos, col_pos)')
            writer.gen_return_py_none()
        self.add_method('insert_elem',
                        func_body=meth_insert_elem,
                        arg_list=[KwdArg(),
                                  UlongArg(name='row_pos',
                                           cvarname='row_pos'),
                                  UlongArg(name='col_pos',
                                           cvarname='col_pos')],
                        doc_str='insert element')

        def get_row_size(writer):
            writer.gen_return_py_ulong('val.row_size()')
        self.add_getter('get_row_size',
                        func_body=get_row_size)
        self.add_attr('row_size',
                      getter_name='get_row_size')

        def get_col_size(writer):
            writer.gen_return_py_ulong('val.col_size()')
        self.add_getter('get_col_size',
                        func_body=get_col_size)
        self.add_attr('col_size',
                      getter_name='get_col_size')

        def meth_col_cost(writer):
            writer.gen_return_py_ulong('val.col_cost(col_pos)')
        self.add_method('col_cost',
                        func_body=meth_col_cost,
                        arg_list=[UlongArg(name='col_pos',
                                           cvarname='col_pos')],
                        doc_str='return column cost')

        def get_col_cost_array(writer):
            writer.gen_return_pyobject('PyList<SizeType, PyUlong>',
                                       'val.col_cost_array()')
        self.add_getter('get_col_cost_array',
                        func_body=get_col_cost_array)
        self.add_attr('col_cost_array',
                      getter_name='get_col_cost_array')

        def meth_solve(writer):
            writer.gen_vardecl(typename='std::vector<SizeType>',
                               varname='solution')
            writer.gen_auto_assign('cost', 'val.solve(solution, option)')
            writer.gen_auto_assign('solution_obj',
                                   'PyList<SizeType, PyUlong>::ToPyObject(solution)')
            writer.gen_return_buildvalue('(Ok)', ['solution_obj', 'cost'])
        self.add_method('solve',
                        func_body=meth_solve,
                        arg_list=[OptArg(),
                                  KwdArg(),
                                  JsonValueArg(name='option',
                                               cvarname='option')],
                        doc_str='solve minimum covering problem')
